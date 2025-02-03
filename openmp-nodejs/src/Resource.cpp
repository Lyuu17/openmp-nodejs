
#include "Resource.hpp"
#include "ResourceManager.hpp"

#include "Utils.hpp"
#include "NodejsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/VehicleComponent.hpp"

Resource::Resource(const std::filesystem::path& folderPath, const std::string& folderName, const std::string& packageJsonBuf)
    : m_folderPath(folderPath)
    , m_folderName(folderName)
{
    m_isolate = node::NewIsolate(NodejsComponent::getInstance()->m_bufferAllocator, uv_default_loop(), NodejsComponent::getInstance()->m_platform.get());
    m_isolate->SetFatalErrorHandler([](const char* location, const char* message) { PRINTLN("%s: %s", location, message); });

    v8::HandleScope handle_scope(m_isolate);

    v8::Local<v8::Context> context = v8::Context::New(m_isolate, nullptr, v8::ObjectTemplate::New(m_isolate));
    m_context.Reset(m_isolate, context);

    // init
    {
        AddFunction("print", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            auto v8str = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
            if (v8str.IsEmpty())
                return;

            PRINTLN("{}", Utils::strV8(v8str.ToLocalChecked()));
        });

        AddFunction("on", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            auto isolate = info.GetIsolate();

            auto resource = (Resource*)info.Data().As<v8::External>()->Value();

            auto v8str  = info[0]->ToString(isolate->GetCurrentContext());
            auto v8func = info[1].As<v8::Function>();
            if (v8str.IsEmpty() || v8func.IsEmpty())
                return;

            resource->AddListener(Utils::strV8(v8str.ToLocalChecked()), v8func);
        }, this);

        AddFunction("sendClientMessageToAll", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            auto isolate = info.GetIsolate();

            auto resource = (Resource*)info.Data().As<v8::External>()->Value();

            auto v8messageColor = info[0]->ToInt32(isolate->GetCurrentContext());
            auto v8messageStr   = info[1]->ToString(isolate->GetCurrentContext());
            if (v8messageStr.IsEmpty() || v8messageColor.IsEmpty())
                return;

            Colour color = Colour::FromRGBA(v8messageColor.ToLocalChecked()->Int32Value(isolate->GetCurrentContext()).ToChecked());

            NodejsComponent::getInstance()->getCore()->getPlayers().sendClientMessageToAll(color, Utils::strV8(v8messageStr.ToLocalChecked()).c_str());
        }, this);
    }

    // init components
    {
        v8::Isolate::Scope iscope(m_isolate);
        v8::HandleScope    handle_scope(m_isolate);

        auto               context = m_context.Get(m_isolate);
        v8::Context::Scope context_scope(context);

        PlayerComponent::InitFunctions();
        VehicleComponent::InitFunctions();
    }

    // parse package json
    auto packageJson = json::parse(packageJsonBuf);
    if (packageJson.contains("main") && packageJson["main"].is_string())
    {
        auto mainStr { packageJson["main"].get<std::string>() };
        auto srcPath { folderPath / mainStr };

        CompileModuleFromFile(srcPath);
    }
}

Resource::~Resource()
{
    while (m_isolate->IsInUse())
        m_isolate->Exit();

    m_modules.clear();
    m_listeners.clear();
    m_objectsFromExtension.clear();

    m_context.Clear();

    NodejsComponent::getInstance()->m_platform->UnregisterIsolate(m_isolate);

    m_isolate->Dispose();
}

void Resource::AddListener(const std::string& name, v8::Local<v8::Function> listener)
{
    m_listeners[name].emplace_back(v8::Global<v8::Function>(m_isolate, listener));
}

void Resource::Emit(const std::string& name, std::initializer_list<v8::Local<v8::Value>> values)
{
    if (!m_listeners.contains(name))
        return;

    for (auto& function : m_listeners[name])
    {
        if (function.IsEmpty())
            return;

        auto localfunction = function.Get(m_isolate);

        auto ctx = m_isolate->GetCurrentContext();

        v8::TryCatch trycatch(m_isolate);

        std::vector<v8::Local<v8::Value>> args(values);
        localfunction->CallAsFunction(ctx, ctx->Global(), args.size(), args.data());

        ReportException(&trycatch);
    }
}

void Resource::AddFunction(const std::string& name, v8::FunctionCallback cb, void* userdata)
{
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);

    auto               context = m_context.Get(m_isolate);
    v8::Context::Scope context_scope(context);

    auto func = v8::Function::New(context, cb, userdata ? v8::External::New(m_isolate, userdata).As<v8::Value>() : v8::Local<v8::Value>());

    context->Global()->Set(context, Utils::v8Str(name), func.ToLocalChecked());
}

bool Resource::CompileModuleFromFile(const std::filesystem::path& modulePath)
{
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);

    auto               context = m_context.Get(m_isolate);
    v8::Context::Scope context_scope(context);

    v8::TryCatch trycatch(m_isolate);

    v8::MaybeLocal<v8::Module> maybeMod = CompileModule(modulePath);
    if (maybeMod.IsEmpty())
    {
        ReportException(&trycatch);
        return false;
    }

    ReportException(&trycatch);

    return true;
}

v8::MaybeLocal<v8::Module> Resource::CompileModule(const std::filesystem::path& modulePath)
{
    const auto modulePathNormalized = std::filesystem::path { modulePath }.lexically_normal();

    std::ifstream moduleStream { modulePathNormalized };
    std::string   moduleContent;
    if (std::filesystem::is_directory(modulePathNormalized))
    {
        moduleStream.open(modulePathNormalized / "index.js");
        if (!moduleStream.is_open())
            return {};
    }
    else if (std::filesystem::is_regular_file(modulePathNormalized))
    {
        moduleStream.open(modulePathNormalized);
        if (!moduleStream.is_open())
            return {};
    }
    else if (!std::filesystem::exists(modulePathNormalized))
    {
        if (std::filesystem::exists(modulePathNormalized.string() + ".js"))
        {
            moduleStream.open(modulePathNormalized.string() + ".js");
            if (!moduleStream.is_open())
                return {};
        }
        else if (std::filesystem::exists(modulePathNormalized.string() + ".mjs"))
        {
            moduleStream.open(modulePathNormalized.string() + ".js");
            if (!moduleStream.is_open())
                return {};
        }
    }

    moduleContent = std::string((std::istreambuf_iterator<char>(moduleStream)), std::istreambuf_iterator<char>());

    if (m_modules.contains(modulePathNormalized))
        return v8::MaybeLocal<v8::Module>(m_modules[modulePathNormalized].Get(m_isolate));

    auto moduleSrc = v8::String::NewFromUtf8(m_isolate, moduleContent.c_str());
    if (moduleSrc.IsEmpty())
        return {};

    v8::ScriptOrigin origin(Utils::v8Str(modulePathNormalized.string()), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true);

    v8::ScriptCompiler::Source source(moduleSrc.ToLocalChecked(), origin);

    auto maybeModule = v8::ScriptCompiler::CompileModule(m_isolate, &source);
    if (!maybeModule.IsEmpty())
    {
        // store the module
        m_modules.insert_or_assign(modulePathNormalized, v8::Global<v8::Module>(m_isolate, maybeModule.ToLocalChecked()));
    }
    return maybeModule;
}

void Resource::InstantiateModule()
{
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);

    auto               context = m_context.Get(m_isolate);
    v8::Context::Scope context_scope(context);

    v8::TryCatch trycatch(m_isolate);

    for (auto const& [name, module] : m_modules)
    {
        auto localmodule = module.Get(m_isolate);

        auto result = localmodule->InstantiateModule(context, ResourceManager::ResolveModule);
        if (result.IsNothing() || !result.IsJust())
        {
            ReportException(&trycatch);
            continue;
        }

        auto value = localmodule->Evaluate(context);
        if (value.IsEmpty())
        {
            ReportException(&trycatch);
            continue;
        }

        if (localmodule->GetStatus() == v8::Module::kErrored)
        {
            v8::Local<v8::Value> exception = localmodule->GetException();
            m_isolate->ThrowException(exception); // rethrow the exception occured when evaluating the module otherwise it won't be thrown
        }
    }

    ReportException(&trycatch);
}

void Resource::ReportException(v8::TryCatch* try_catch)
{
    if (!try_catch->HasCaught())
    {
        return;
    }

    auto message = try_catch->Message();
    auto context = m_isolate->GetCurrentContext();

    v8::String::Utf8Value exception(m_isolate, try_catch->Exception());
    LOGLN(LogLevel::Error, "Exception: {}", *exception);

    // Log message details (if available)
    if (!message.IsEmpty())
    {
        v8::String::Utf8Value filename(m_isolate, message->GetScriptOrigin().ResourceName());
        int                   linenum = message->GetLineNumber(context).FromJust();
        v8::String::Utf8Value exception(m_isolate, message->Get());

        LOGLN(LogLevel::Error, "{}:{}: {}", *filename, linenum, *exception);

        Utils::PrintSourceLineWithUnderline(m_isolate, message, context);
    }

    v8::Local<v8::Value> stackTraceString;
    if (try_catch->StackTrace(context).ToLocal(&stackTraceString) &&
        stackTraceString->IsString() &&
        v8::Local<v8::String>::Cast(stackTraceString)->Length() > 0)
    {
        v8::String::Utf8Value stackTrace(m_isolate, stackTraceString);
        LOGLN(LogLevel::Error, "Stack trace: {}", *stackTrace);
    }
}

v8::MaybeLocal<v8::Module> Resource::ResolveFile(const std::string& importPath, v8::Local<v8::Module> referrer)
{
    std::filesystem::path modulePath {};
    for (auto& [path, mod] : m_modules)
    {
        if (referrer == mod)
        {
            modulePath = (path / importPath).lexically_normal();
            break;
        }
    }

    if (modulePath.empty())
        return {};

    if (!m_modules.contains(modulePath))
    {
        return CompileModule(modulePath);
    }

    return m_modules.at(modulePath).Get(m_isolate);
}

void Resource::ThrowException(const std::string& text)
{
    m_isolate->ThrowException(v8::String::NewFromUtf8(m_isolate, text.c_str()).ToLocalChecked());
}

void Resource::Exec(std::function<void(Resource* resource)> func)
{
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);

    auto               context = m_context.Get(m_isolate);
    v8::Context::Scope context_scope(context);

    func(this);
}

void Resource::RemoveExtension(IJavaScriptClassExtension* ext)
{
    m_objectsFromExtension.erase(ext);
}

bool Resource::DoesObjectFromExtensionExist(IJavaScriptClassExtension* ext)
{
    return m_objectsFromExtension.contains(ext);
}

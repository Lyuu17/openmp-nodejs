
#include "Resource.hpp"
#include "ResourceManager.hpp"

#include "Utils.hpp"
#include "NodejsComponent.hpp"
#include "CommonFunctions.hpp"
#include "components/ClassesComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/VehicleComponent.hpp"
#include "components/PickupComponent.hpp"
#include "components/MenuComponent.hpp"
#include "components/TextdrawComponent.hpp"

#include "bootstrapjs.hpp"

Resource::Resource(node::MultiIsolatePlatform* platform, const std::filesystem::path& folderPath, const std::string& folderName, const std::string& packageJsonBuf)
    : m_folderPath(folderPath)
    , m_folderName(folderName)
    , m_packageBuf(packageJsonBuf)
{
    uvLoop = new uv_loop_t;
    uv_loop_init(uvLoop);

    auto allocator = node::CreateArrayBufferAllocator();
    m_isolate      = node::NewIsolate(allocator, uvLoop, platform);
    nodeData       = node::CreateIsolateData(m_isolate, uvLoop, platform, allocator);

    v8::Locker         locker(m_isolate);
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);

    auto context = node::NewContext(m_isolate, v8::ObjectTemplate::New(m_isolate));
    m_context.Reset(m_isolate, context);
}

Resource::~Resource()
{
    v8::Locker         locker(m_isolate);
    v8::Isolate::Scope isolateScope(m_isolate);
    v8::HandleScope    handleScope(m_isolate);
    v8::Context::Scope context_scope(GetContext());

    node::EmitAsyncDestroy(m_isolate, m_asyncContext);
    m_asyncResource.Reset();

    m_modules.clear();
    m_listeners.clear();
    m_objectsFromExtension.clear();

    node::EmitProcessBeforeExit(env);
    node::EmitProcessExit(env);

    node::Stop(env);

    node::FreeEnvironment(env);
    node::FreeIsolateData(nodeData);

    uv_loop_close(uvLoop);
    delete uvLoop;

    m_isolate->Dispose();

    m_envStarted = false;
}

void Resource::Start(node::MultiIsolatePlatform* platform, node::Environment* parentEnv)
{
    v8::Locker         locker(m_isolate);
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);
    v8::Context::Scope context_scope(GetContext());

    // init
    {
        CommonFunctions::Init(this);
    }

    // init components
    {
        ClassesComponent::InitFunctions(this);
        PlayerComponent::InitFunctions(this);
        VehicleComponent::InitFunctions(this);
        PickupComponent::InitFunctions(this);
        MenuComponent::InitFunctions(this);
        TextdrawComponent::InitFunctions(this);
    }

    // parse package json
    std::filesystem::path mainPath;

    auto packageJson = json::parse(m_packageBuf);
    {
        if (packageJson.contains("main") && packageJson["main"].is_string())
        {
            mainPath = packageJson["main"].get<std::string>();
        }

        m_name = packageJson.contains("name") && packageJson["name"].is_string()
                     ? packageJson["name"].get<std::string>()
                     : m_folderName;
    }

    node::ThreadId threadId  = node::AllocateEnvironmentThreadId();
    auto           flags     = static_cast<node::EnvironmentFlags::Flags>(node::EnvironmentFlags::kNoFlags);
    auto           inspector = node::GetInspectorParentHandle(parentEnv, threadId, m_name.c_str());

    std::vector<std::string> args { m_name };
    std::vector<std::string> exec_args {};

    auto context = m_isolate->GetCurrentContext();

    auto resourceObj = v8::Object::New(m_isolate);
    resourceObj->Set(context, Utils::v8Str("path"), Utils::v8Str(m_folderPath.string()));
    resourceObj->Set(context, Utils::v8Str("main"), Utils::v8Str(mainPath.string()));
    resourceObj->Set(context, Utils::v8Str("name"), Utils::v8Str(m_name));

    context->Global()->Set(context, Utils::v8Str("resource"), resourceObj);

    AddFunction("__resourceLoaded", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        resource->m_envStarted = true;

        if (!info[0]->IsNullOrUndefined())
            resource->m_exports.Reset(info.GetIsolate(), info[0].As<v8::Value>());

        auto& resourceName = resource->m_name;
        NodejsComponent::getInstance()->getResourceManager()->Exec([resourceName](Resource* resource) {
            resource->Emit("onResourceStart", { Utils::v8Str(resourceName) });
        });

        PRINTLN("Resource loaded: {}", resourceName);
    }, this);

    env = node::CreateEnvironment(nodeData, context, args, exec_args, flags, threadId, std::move(inspector));
    node::LoadEnvironment(env, bootstrapjs);

    m_asyncResource.Reset(m_isolate, v8::Object::New(m_isolate));
    m_asyncContext = node::EmitAsyncInit(m_isolate, m_asyncResource.Get(m_isolate), m_name.c_str());

    while (!m_envStarted)
    {
        OnTick(platform);
    }
}

void Resource::Stop()
{
    auto& resourceName = m_name;
    NodejsComponent::getInstance()->getResourceManager()->Exec([resourceName](Resource* resource) {
        resource->Emit("onResourceStop", { Utils::v8Str(resourceName) }, true);
    });
}

void Resource::OnTick(node::MultiIsolatePlatform* platform)
{
    v8::Locker         locker(m_isolate);
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);
    v8::Context::Scope context_scope(GetContext());

    // run the async otherwise promises won't be fullfilled ever
    node::CallbackScope callbackScope(m_isolate, m_asyncResource.Get(m_isolate), m_asyncContext);

    platform->DrainTasks(m_isolate);

    uv_run(uvLoop, UV_RUN_NOWAIT);
}

void Resource::AddListener(const std::string& name, v8::Local<v8::Function> listener)
{
    m_listeners[name].emplace_back(v8::Global<v8::Function>(m_isolate, listener));
}

void Resource::Emit(const std::string& name, const std::vector<v8::Local<v8::Value>>& values, bool waitAsync)
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

        auto v8result = localfunction->Call(ctx, ctx->Global(), args.size(), args.data());
        if (!v8result.IsEmpty() && v8result.ToLocalChecked()->IsPromise() && waitAsync)
        {
            auto v8promise = v8result.ToLocalChecked().As<v8::Promise>();

            while (v8promise->State() == v8::Promise::kPending)
            {
                NodejsComponent::getInstance()->getResourceManager()->Tick();

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        ReportException(&trycatch);
    }
}

void Resource::AddFunction(const std::string& name, v8::FunctionCallback cb, void* userdata)
{
    auto context = m_isolate->GetCurrentContext();

    auto func = v8::Function::New(context, cb, userdata ? v8::External::New(m_isolate, userdata).As<v8::Value>() : v8::Local<v8::Value>());

    context->Global()->Set(context, Utils::v8Str(name), func.ToLocalChecked());
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

void Resource::ThrowException(const std::string& text)
{
    m_isolate->ThrowException(v8::String::NewFromUtf8(m_isolate, text.c_str()).ToLocalChecked());
}

void Resource::Exec(std::function<void(Resource* resource)> func)
{
    v8::Locker         locker(m_isolate);
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);
    v8::Context::Scope context_scope(GetContext());

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

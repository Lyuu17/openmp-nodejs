#include "ResourceManager.hpp"

#include "NodejsComponent.hpp"

ResourceManager::ResourceManager()
{
    std::vector<std::string> args { "omp-server",
        "--no-warnings",
        "--expose-internals",
        /*"--inspect=127.0.0.1:5858"*/ };

    auto result = node::InitializeOncePerProcess(args);

    m_platform.reset(result->platform());

    auto allocator              = node::CreateArrayBufferAllocator();
    m_isolate                   = node::NewIsolate(allocator, uv_default_loop(), m_platform.get());
    node::IsolateData* nodeData = node::CreateIsolateData(m_isolate, uv_default_loop(), m_platform.get(), allocator);

    {
        v8::Locker         locker(m_isolate);
        v8::Isolate::Scope isolate_scope(m_isolate);
        v8::HandleScope    handle_scope(m_isolate);

        m_context.Reset(m_isolate, node::NewContext(m_isolate));

        auto context = m_context.Get(m_isolate);

        v8::Context::Scope scope(context);

        m_parentEnv = node::CreateEnvironment(nodeData, context, result->args(), result->exec_args());

        /*
            Load here only needs for debugging as this environment only used as a parent for real environments
        */

        // node::LoadEnvironment(parentEnv, "console.log('PARENT INIT'); setInterval(() => {}, 1000);");
    }
}

ResourceManager::~ResourceManager()
{
    m_resources.clear();

    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    node::TearDownOncePerProcess();
}

void ResourceManager::freeExtension()
{
    delete this;
}

void ResourceManager::reset()
{
}

void ResourceManager::Tick()
{
    v8::Locker         locker(m_isolate);
    v8::Isolate::Scope iscope(m_isolate);
    v8::HandleScope    handle_scope(m_isolate);
    v8::Context::Scope context_scope(m_context.Get(m_isolate));

    uv_run(uv_default_loop(), UV_RUN_NOWAIT);

    m_platform->DrainTasks(m_isolate);

    for (auto& [path, resource] : m_resources)
        resource->OnTick(m_platform.get());
}

void ResourceManager::LoadResourcesFromPath(const std::filesystem::path& path)
{
    std::filesystem::create_directories(path);

    for (auto const& dir_entry : std::filesystem::directory_iterator { path })
        LoadResource(dir_entry);
}

void ResourceManager::LoadResource(const std::filesystem::path& path)
{
    std::ifstream packageJson { path / "package.json" };
    if (!packageJson.is_open())
    {
        LOGLN(LogLevel::Error, "Failed to load resource folder {}, missing package.json", path.string());
        return;
    }

    std::string packageJsonContent((std::istreambuf_iterator<char>(packageJson)), std::istreambuf_iterator<char>());

    const auto folderName = path.filename().string();

    auto resource = std::make_unique<Resource>(m_platform.get(), path, folderName, packageJsonContent);

    auto [insertPair, result] = m_resources.emplace(path, std::move(resource));

    insertPair->second->Start(m_platform.get(), m_parentEnv);
    insertPair->second->Exec([](Resource* resource) {
        resource->Emit("OnResourceLoad", {});
    });

    PRINTLN("Found resource: {}", folderName);
}

void ResourceManager::Exec(std::function<void(Resource* resource)> func)
{
    for (auto const& [path, resource] : m_resources)
        resource->Exec(func);
}

Resource* ResourceManager::GetResourceFromIsolate(v8::Isolate* isolate)
{
    for (auto const& [path, resource] : m_resources)
    {
        if (resource->m_isolate == isolate)
            return resource.get();
    }
    return nullptr;
}

#include "ResourceManager.hpp"

#include "NodejsComponent.hpp"

ResourceManager::ResourceManager()
{
    std::vector<std::string> args { "omp-server",
        "--no-warnings",
        "--expose-internals",
        /*"--inspect=127.0.0.1:5858"*/ };

    auto result = node::InitializeOncePerProcess(args);

    m_platform = result->platform();

    m_allocator = node::CreateArrayBufferAllocator();
    m_isolate   = node::NewIsolate(m_allocator, uv_default_loop(), m_platform);
    m_nodeData  = node::CreateIsolateData(m_isolate, uv_default_loop(), m_platform, m_allocator);

    {
        v8::Locker         locker(m_isolate);
        v8::Isolate::Scope isolate_scope(m_isolate);
        v8::HandleScope    handle_scope(m_isolate);

        m_context.Reset(m_isolate, node::NewContext(m_isolate));

        auto context = m_context.Get(m_isolate);

        v8::Context::Scope scope(context);

        m_parentEnv = node::CreateEnvironment(m_nodeData, context, result->args(), result->exec_args());

        /*
            Load here only needs for debugging as this environment only used as a parent for real environments
        */

        // node::LoadEnvironment(m_parentEnv, "console.log('PARENT INIT'); setInterval(() => {}, 1000);");
    }
}

ResourceManager::~ResourceManager()
{
    for (auto& [path, resource] : m_resources)
        resource->Stop();

    m_resources.clear();

    // clean up parent resource
    {
        {
            v8::Locker         locker(m_isolate);
            v8::Isolate::Scope isolateScope(m_isolate);

            v8::HandleScope    handleScope(m_isolate);
            v8::Context::Scope context_scope(m_context.Get(m_isolate));

            node::EmitProcessBeforeExit(m_parentEnv);
            node::EmitProcessExit(m_parentEnv);

            node::Stop(m_parentEnv);

            // node::FreeEnvironment(m_parentEnv);
            node::FreeIsolateData(m_nodeData);

            m_context.Reset();
        }

        bool platform_finished = false;
        m_platform->AddIsolateFinishedCallback(m_isolate, [](void* data) {
            *static_cast<bool*>(data) = true;
        }, &platform_finished);
        m_platform->UnregisterIsolate(m_isolate);

        m_isolate->Dispose();

        // Wait until the platform has cleaned up all relevant resources.
        while (!platform_finished)
            uv_run(uv_default_loop(), UV_RUN_ONCE);

        node::FreeArrayBufferAllocator(m_allocator);
    }

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
        resource->OnTick(m_platform);
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

    auto resource = std::make_unique<Resource>(m_platform, path, folderName, packageJsonContent);

    auto [insertPair, result] = m_resources.emplace(path, std::move(resource));

    insertPair->second->Start(m_platform, m_parentEnv);
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

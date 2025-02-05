
#include <Server/Components/Classes/classes.hpp>

#include "NodejsComponent.hpp"

#include "components/PlayerComponent.hpp"
#include "components/events/ClassEventsComponent.hpp"
#include "components/events/PlayerEventsComponent.hpp"
#include "components/events/VehicleEventsComponent.hpp"

#include "ResourceManager.hpp"

NodejsComponent* NodejsComponent::m_instance = nullptr;

NodejsComponent::~NodejsComponent()
{
    if (m_core)
    {
        m_core->getEventDispatcher().removeEventHandler(this);
    }
}

StringView NodejsComponent::componentName() const
{
    return "nodejs";
}

SemanticVersion NodejsComponent::componentVersion() const
{
    return { 0, 1, 0, 0 };
}

void NodejsComponent::onLoad(ICore* core)
{
    m_core = core;
    m_core->getEventDispatcher().addEventHandler(this);

    std::vector<std::string> args { "omp-server",
        "--no-warnings",
        "--expose-internals",
        /*"--inspect=127.0.0.1:5858"*/ };

    auto result = node::InitializeOncePerProcess(args);

    m_platform.reset(result->platform());

    auto allocator              = node::CreateArrayBufferAllocator();
    isolate                     = node::NewIsolate(allocator, uv_default_loop(), m_platform.get());
    node::IsolateData* nodeData = node::CreateIsolateData(isolate, uv_default_loop(), m_platform.get(), allocator);
    {
        v8::Locker         locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope    handle_scope(isolate);

        context.Reset(isolate, node::NewContext(isolate));
        v8::Context::Scope scope(context.Get(isolate));

        parentEnv = node::CreateEnvironment(nodeData, context.Get(isolate), result->args(), result->exec_args());

        /*
            Load here only needs for debugging as this environment only used as a parent for real environments
        */

        node::LoadEnvironment(parentEnv, "console.log('PARENT INIT'); setInterval(() => {}, 1000);");
    }
}

void NodejsComponent::onInit(IComponentList* components)
{
    m_classes   = components->queryComponent<IClassesComponent>();
    m_databases = components->queryComponent<IDatabasesComponent>();
    m_vehicles  = components->queryComponent<IVehiclesComponent>();

    addExtension(new ClassEventsComponent(m_core, m_classes), true);
    addExtension(new PlayerEventsComponent(m_core), true);
    addExtension(new VehicleEventsComponent(m_core, m_vehicles), true);

    ResourceManager::LoadResourcesFromPath("resources");
}

void NodejsComponent::free()
{
    ResourceManager::Deinit();

    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    node::TearDownOncePerProcess();

    delete this;
}

void NodejsComponent::reset()
{
}

void NodejsComponent::onTick(std::chrono::microseconds elapsed, std::chrono::steady_clock::time_point now)
{
    v8::Locker         locker(isolate);
    v8::Isolate::Scope iscope(isolate);
    v8::HandleScope    handle_scope(isolate);
    v8::Context::Scope context_scope(context.Get(isolate));

    uv_run(uv_default_loop(), UV_RUN_NOWAIT);

    NodejsComponent::getInstance()->m_platform->DrainTasks(isolate);

    ResourceManager::Tick();
}

ICore* NodejsComponent::getCore()
{
    return m_core;
}

IClassesComponent* NodejsComponent::getClasses()
{
    return m_classes;
}

IDatabasesComponent* NodejsComponent::getDatabases()
{
    return m_databases;
}

IVehiclesComponent* NodejsComponent::getVehicles()
{
    return m_vehicles;
}

NodejsComponent* NodejsComponent::getInstance()
{
    if (m_instance == nullptr)
        m_instance = new NodejsComponent();

    return m_instance;
}

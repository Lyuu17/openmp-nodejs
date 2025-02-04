
#include <Server/Components/Classes/classes.hpp>

#include "NodejsComponent.hpp"

#include "components/PlayerComponent.hpp"
#include "components/events/ClassEventsComponent.hpp"
#include "components/events/PlayerEventsComponent.hpp"
#include "components/events/VehicleEventsComponent.hpp"
#include "extensions/DbExtension.hpp"

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

    auto result = node::InitializeOncePerProcess({}, { node::ProcessInitializationFlags::kDisableNodeOptionsEnv, node::ProcessInitializationFlags::kDisableCLIOptions });

    m_platform.reset(result->platform());

    m_bufferAllocator = node::CreateArrayBufferAllocator();
}

void NodejsComponent::onInit(IComponentList* components)
{
    m_classes   = components->queryComponent<IClassesComponent>();
    m_databases = components->queryComponent<IDatabasesComponent>();
    m_vehicles  = components->queryComponent<IVehiclesComponent>();

    addExtension(new ClassEventsComponent(m_core, m_classes), true);
    addExtension(new PlayerEventsComponent(m_core), true);
    addExtension(new VehicleEventsComponent(m_core, m_vehicles), true);

    addExtension(new DbExtension(m_core, m_databases), true);

    ResourceManager::LoadResourcesFromPath("resources");
}

void NodejsComponent::free()
{
    ResourceManager::Deinit();

    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    node::FreeArrayBufferAllocator(m_bufferAllocator);

    delete this;
}

void NodejsComponent::reset()
{
}

void NodejsComponent::onTick(std::chrono::microseconds elapsed, std::chrono::steady_clock::time_point now)
{
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

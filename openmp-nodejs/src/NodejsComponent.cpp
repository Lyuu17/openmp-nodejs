
#include "NodejsComponent.hpp"

#include "components/PlayerComponent.hpp"
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

    addExtension(new PlayerEventsComponent(core), true);

    auto result = node::InitializeOncePerProcess({}, { node::ProcessInitializationFlags::kDisableNodeOptionsEnv, node::ProcessInitializationFlags::kDisableCLIOptions });

    m_platform.reset(result->platform());

    m_bufferAllocator = node::CreateArrayBufferAllocator();

    ResourceManager::LoadResourcesFromPath("resources");
}

void NodejsComponent::onInit(IComponentList* components)
{
    m_vehicles = components->queryComponent<IVehiclesComponent>();

    addExtension(new VehicleEventsComponent(m_core, m_vehicles), true);
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

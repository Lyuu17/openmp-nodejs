
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
}

void NodejsComponent::onInit(IComponentList* components)
{
    addExtension(new ResourceManager(), true);

    m_classes  = components->queryComponent<IClassesComponent>();
    m_vehicles = components->queryComponent<IVehiclesComponent>();

    m_resourceManager = queryExtension<ResourceManager>(this);
    m_resourceManager->LoadResourcesFromPath("resources");

    addExtension(new ClassEventsComponent(m_core, m_classes, m_resourceManager), true);
    addExtension(new PlayerEventsComponent(m_core, m_resourceManager), true);
    addExtension(new VehicleEventsComponent(m_core, m_vehicles, m_resourceManager), true);
}

void NodejsComponent::free()
{
    delete this;
}

void NodejsComponent::reset()
{
}

void NodejsComponent::onTick(std::chrono::microseconds elapsed, std::chrono::steady_clock::time_point now)
{
    m_resourceManager->Tick();
}

ICore* NodejsComponent::getCore()
{
    return m_core;
}

IClassesComponent* NodejsComponent::getClasses()
{
    return m_classes;
}

IVehiclesComponent* NodejsComponent::getVehicles()
{
    return m_vehicles;
}

ResourceManager* NodejsComponent::getResourceManager()
{
    return m_resourceManager;
}

NodejsComponent* NodejsComponent::getInstance()
{
    if (m_instance == nullptr)
        m_instance = new NodejsComponent();

    return m_instance;
}

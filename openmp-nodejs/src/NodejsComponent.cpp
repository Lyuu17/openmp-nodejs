
#include <Server/Components/Classes/classes.hpp>

#include "NodejsComponent.hpp"

#include "components/PlayerComponent.hpp"
#include "components/events/ClassEventsComponent.hpp"
#include "components/events/PlayerEventsComponent.hpp"
#include "components/events/VehicleEventsComponent.hpp"
#include "components/events/PickupEventsComponent.hpp"
#include "components/events/MenuEventsComponent.hpp"
#include "components/events/DialogEventsComponent.hpp"
#include "components/events/CheckpointEventsComponent.hpp"
#include "components/events/TextdrawEventsComponent.hpp"

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

    m_classes     = components->queryComponent<IClassesComponent>();
    m_vehicles    = components->queryComponent<IVehiclesComponent>();
    m_pickups     = components->queryComponent<IPickupsComponent>();
    m_menus       = components->queryComponent<IMenusComponent>();
    m_dialogs     = components->queryComponent<IDialogsComponent>();
    m_checkpoints = components->queryComponent<ICheckpointsComponent>();
    m_textdraws   = components->queryComponent<ITextDrawsComponent>();

    m_resourceManager = queryExtension<ResourceManager>(this);
    m_resourceManager->LoadResourcesFromPath("resources");

    addExtension(new ClassEventsComponent(m_core, m_classes, m_resourceManager), true);
    addExtension(new PlayerEventsComponent(m_core, m_resourceManager), true);
    addExtension(new VehicleEventsComponent(m_core, m_vehicles, m_resourceManager), true);
    addExtension(new PickupEventsComponent(m_core, m_pickups, m_resourceManager), true);
    addExtension(new MenuEventsComponent(m_core, m_menus, m_resourceManager), true);
    addExtension(new DialogEventsComponent(m_core, m_dialogs, m_resourceManager), true);
    addExtension(new CheckpointEventsComponent(m_core, m_checkpoints, m_resourceManager), true);
    addExtension(new TextdrawEventsComponent(m_core, m_textdraws, m_resourceManager), true);
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

IPickupsComponent* NodejsComponent::getPickups()
{
    return m_pickups;
}

IMenusComponent* NodejsComponent::getMenus()
{
    return m_menus;
}

IDialogsComponent* NodejsComponent::getDialogs()
{
    return m_dialogs;
}

ITextDrawsComponent* NodejsComponent::getTextDraws()
{
    return m_textdraws;
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

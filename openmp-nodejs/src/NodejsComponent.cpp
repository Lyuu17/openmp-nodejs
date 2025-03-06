
#include <Server/Components/Classes/classes.hpp>

#include "NodejsComponent.hpp"

#include "classes/PlayerExtension.hpp"
#include "events/ClassEventsExtension.hpp"
#include "events/PlayerEventsExtension.hpp"
#include "events/VehicleEventsExtension.hpp"
#include "events/PickupEventsExtension.hpp"
#include "events/MenuEventsExtension.hpp"
#include "events/DialogEventsExtension.hpp"
#include "events/CheckpointEventsExtension.hpp"
#include "events/TextdrawEventsExtension.hpp"
#include "events/ObjectEventsExtension.hpp"
#include "events/CustomModelEventsExtension.hpp"

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

    m_classes      = components->queryComponent<IClassesComponent>();
    m_vehicles     = components->queryComponent<IVehiclesComponent>();
    m_pickups      = components->queryComponent<IPickupsComponent>();
    m_menus        = components->queryComponent<IMenusComponent>();
    m_dialogs      = components->queryComponent<IDialogsComponent>();
    m_checkpoints  = components->queryComponent<ICheckpointsComponent>();
    m_textdraws    = components->queryComponent<ITextDrawsComponent>();
    m_objects      = components->queryComponent<IObjectsComponent>();
    m_customModels = components->queryComponent<ICustomModelsComponent>();
    m_pawn         = components->queryComponent<IPawnComponent>();

    addExtension(new ClassEventsExtension(m_core, m_classes, m_resourceManager), true);
    addExtension(new PlayerEventsExtension(m_core, m_resourceManager), true);
    addExtension(new VehicleEventsExtension(m_core, m_vehicles, m_resourceManager), true);
    addExtension(new PickupEventsExtension(m_core, m_pickups, m_resourceManager), true);
    addExtension(new MenuEventsExtension(m_core, m_menus, m_resourceManager), true);
    addExtension(new DialogEventsExtension(m_core, m_dialogs, m_resourceManager), true);
    addExtension(new CheckpointEventsExtension(m_core, m_checkpoints, m_resourceManager), true);
    addExtension(new TextDrawEventsExtension(m_core, m_textdraws, m_resourceManager), true);
    addExtension(new ObjectEventsExtension(m_core, m_objects, m_resourceManager), true);
    addExtension(new CustomModelEventsExtension(m_core, m_customModels, m_resourceManager), true);
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
    if (!m_loaded)
    {
        m_resourceManager = queryExtension<ResourceManager>(this);
        m_resourceManager->LoadResourcesFromPath("resources");

        m_loaded = true;
    }

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

IObjectsComponent* NodejsComponent::getObjects()
{
    return m_objects;
}

IPawnComponent* NodejsComponent::getPawn()
{
    return m_pawn;
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

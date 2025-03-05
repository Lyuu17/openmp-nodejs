
#include "events/PickupEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"
#include "classes/PickupExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

PickupEventsExtension::PickupEventsExtension(ICore* core, IPickupsComponent* pickups, ResourceManager* resourceManager)
    : m_core(core)
    , m_pickups(pickups)
    , m_resourceManager(resourceManager)
{
    if (m_pickups)
    {
        m_pickups->getEventDispatcher().addEventHandler(this);
    }
}

PickupEventsExtension::~PickupEventsExtension()
{
    if (m_core)
    {
    }
}

void PickupEventsExtension::freeExtension()
{
    delete this;
}

void PickupEventsExtension::reset()
{
}

// =========== events

void PickupEventsExtension::onPlayerPickUpPickup(IPlayer& player, IPickup& pickup)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objPickup = resource->ObjectFromExtension(queryExtension<PickupExtension>(pickup));

        resource->Emit("onPlayerPickUpPickup", { v8objPlayer, v8objPickup });
    });
}

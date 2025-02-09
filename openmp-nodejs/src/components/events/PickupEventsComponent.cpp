
#include "components/events/PickupEventsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/PickupComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

PickupEventsComponent::PickupEventsComponent(ICore* core, IPickupsComponent* pickups, ResourceManager* resourceManager)
    : m_core(core)
    , m_pickups(pickups)
    , m_resourceManager(resourceManager)
{
    if (m_pickups)
    {
        m_pickups->getEventDispatcher().addEventHandler(this);
    }
}

PickupEventsComponent::~PickupEventsComponent()
{
    if (m_core)
    {
    }
}

void PickupEventsComponent::freeExtension()
{
    delete this;
}

void PickupEventsComponent::reset()
{
}

// =========== events

void PickupEventsComponent::onPlayerPickUpPickup(IPlayer& player, IPickup& pickup)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objPickup = resource->ObjectFromExtension(queryExtension<PickupComponent>(pickup));

        resource->Emit("onPlayerPickUpPickup", { v8objPlayer, v8objPickup });
    });
}

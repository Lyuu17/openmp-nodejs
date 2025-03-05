
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Pickups/pickups.hpp>
#include <sdk.hpp>

struct PickupEventsExtension final : public IExtension,
                                     public PickupEventHandler
{
    PROVIDE_EXT_UID(0x78AD72CB709E00CC);

    ICore*             m_core            = nullptr;
    IPickupsComponent* m_pickups         = nullptr;
    ResourceManager*   m_resourceManager = nullptr;

public:
    PickupEventsExtension(ICore*, IPickupsComponent* pickups, ResourceManager* resourceManager);
    ~PickupEventsExtension();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onPlayerPickUpPickup(IPlayer& player, IPickup& pickup) override;
};

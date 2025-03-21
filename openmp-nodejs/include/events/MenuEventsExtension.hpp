
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Menus/menus.hpp>
#include <sdk.hpp>

struct MenuEventsExtension final : public IExtension,
                                   public MenuEventHandler
{
    PROVIDE_EXT_UID(0x6DB77A5B7A8732C3);

    ICore*           m_core            = nullptr;
    IMenusComponent* m_menus           = nullptr;
    ResourceManager* m_resourceManager = nullptr;

public:
    MenuEventsExtension(ICore*, IMenusComponent* menus, ResourceManager* resourceManager);
    ~MenuEventsExtension();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onPlayerSelectedMenuRow(IPlayer& player, MenuRow row) override;
    void onPlayerExitedMenu(IPlayer& player) override;
};

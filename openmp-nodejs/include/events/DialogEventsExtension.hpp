
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Dialogs/dialogs.hpp>
#include <sdk.hpp>

struct DialogEventsExtension final : public IExtension,
                                     public PlayerDialogEventHandler
{
    PROVIDE_EXT_UID(0xC9C85D45C3276E5B);

    ICore*             m_core            = nullptr;
    IDialogsComponent* m_dialogs         = nullptr;
    ResourceManager*   m_resourceManager = nullptr;

public:
    DialogEventsExtension(ICore*, IDialogsComponent*, ResourceManager*);
    ~DialogEventsExtension();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onDialogResponse(IPlayer& player, int dialogId, DialogResponse response, int listItem, StringView inputText) override;
};

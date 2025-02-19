
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/TextDraws/textdraws.hpp>
#include <sdk.hpp>

struct TextdrawEventsComponent final : public IExtension,
                                       public TextDrawEventHandler
{
    PROVIDE_EXT_UID(0x48875A29CFD4F6EA);

    ICore*               m_core            = nullptr;
    ITextDrawsComponent* m_textdraws       = nullptr;
    ResourceManager*     m_resourceManager = nullptr;

public:
    TextdrawEventsComponent(ICore*, ITextDrawsComponent*, ResourceManager*);
    ~TextdrawEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onPlayerClickTextDraw(IPlayer& player, ITextDraw& td) override;
    void onPlayerClickPlayerTextDraw(IPlayer& player, IPlayerTextDraw& td) override;
    bool onPlayerCancelTextDrawSelection(IPlayer& player) override;
    bool onPlayerCancelPlayerTextDrawSelection(IPlayer& player) override;
};

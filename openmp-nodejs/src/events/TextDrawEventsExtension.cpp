
#include "events/TextDrawEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"
#include "classes/TextdrawExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

TextDrawEventsExtension::TextDrawEventsExtension(ICore* core, ITextDrawsComponent* textdraw, ResourceManager* resourceManager)
    : m_core(core)
    , m_textdraws(textdraw)
    , m_resourceManager(resourceManager)
{
    if (m_textdraws)
    {
        m_textdraws->getEventDispatcher().addEventHandler(this);
    }
}

TextDrawEventsExtension::~TextDrawEventsExtension()
{
}

void TextDrawEventsExtension::freeExtension()
{
    delete this;
}

void TextDrawEventsExtension::reset()
{
}

// =========== events

void TextDrawEventsExtension::onPlayerClickTextDraw(IPlayer& player, ITextDraw& td)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer   = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objTextdraw = resource->ObjectFromExtension(queryExtension<TextDrawExtension>(td));

        resource->Emit("onPlayerClickTextDraw", { v8objPlayer, v8objTextdraw });
    });
}

void TextDrawEventsExtension::onPlayerClickPlayerTextDraw(IPlayer& player, IPlayerTextDraw& td)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer   = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objTextdraw = resource->ObjectFromExtension(queryExtension<TextDrawExtension>(td));

        resource->Emit("onPlayerClickPlayerTextDraw", { v8objPlayer, v8objTextdraw });
    });
}

bool TextDrawEventsExtension::onPlayerCancelTextDrawSelection(IPlayer& player)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerCancelTextDrawSelection", { v8objPlayer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool TextDrawEventsExtension::onPlayerCancelPlayerTextDrawSelection(IPlayer& player)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerCancelPlayerTextDrawSelection", { v8objPlayer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

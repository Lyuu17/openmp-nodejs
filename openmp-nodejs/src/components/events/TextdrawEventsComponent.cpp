
#include "components/events/TextdrawEventsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/TextdrawComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

TextdrawEventsComponent::TextdrawEventsComponent(ICore* core, ITextDrawsComponent* textdraw, ResourceManager* resourceManager)
    : m_core(core)
    , m_textdraws(textdraw)
    , m_resourceManager(resourceManager)
{
    if (m_textdraws)
    {
        m_textdraws->getEventDispatcher().addEventHandler(this);
    }
}

TextdrawEventsComponent::~TextdrawEventsComponent()
{
}

void TextdrawEventsComponent::freeExtension()
{
    delete this;
}

void TextdrawEventsComponent::reset()
{
}

// =========== events

void TextdrawEventsComponent::onPlayerClickTextDraw(IPlayer& player, ITextDraw& td)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer   = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objTextdraw = resource->ObjectFromExtension(queryExtension<TextdrawComponent>(td));

        resource->Emit("onPlayerClickTextDraw", { v8objPlayer, v8objTextdraw });
    });
}

void TextdrawEventsComponent::onPlayerClickPlayerTextDraw(IPlayer& player, IPlayerTextDraw& td)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer   = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objTextdraw = resource->ObjectFromExtension(queryExtension<TextdrawComponent>(td));

        resource->Emit("onPlayerClickPlayerTextDraw", { v8objPlayer, v8objTextdraw });
    });
}

bool TextdrawEventsComponent::onPlayerCancelTextDrawSelection(IPlayer& player)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerCancelTextDrawSelection", { v8objPlayer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool TextdrawEventsComponent::onPlayerCancelPlayerTextDrawSelection(IPlayer& player)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerCancelPlayerTextDrawSelection", { v8objPlayer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}


#include "components/events/DialogEventsComponent.hpp"
#include "components/PlayerComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

DialogEventsComponent::DialogEventsComponent(ICore* core, IDialogsComponent* dialogs, ResourceManager* resourceManager)
    : m_core(core)
    , m_dialogs(dialogs)
    , m_resourceManager(resourceManager)
{
    if (m_dialogs)
    {
        m_dialogs->getEventDispatcher().addEventHandler(this);
    }
}

DialogEventsComponent::~DialogEventsComponent()
{
}

void DialogEventsComponent::freeExtension()
{
    delete this;
}

void DialogEventsComponent::reset()
{
}

// =========== events

void DialogEventsComponent::onDialogResponse(IPlayer& player, int dialogId, DialogResponse response, int listItem, StringView inputText)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto playerComponent = queryExtension<PlayerComponent>(player);
        if (!playerComponent)
            return;

        if (playerComponent->m_dialogCallback.IsEmpty())
            return;

        auto v8objPlayer = resource->ObjectFromExtension(playerComponent);
        auto v8response  = v8::Integer::New(resource->m_isolate, response);
        auto v8listItem  = v8::Integer::New(resource->m_isolate, listItem);
        auto v8inputText = Utils::v8Str(inputText.data());

        std::vector<v8::Local<v8::Value>> args {
            v8objPlayer, v8response, v8listItem, v8inputText
        };

        auto ctx = resource->m_isolate->GetCurrentContext();

        auto dialogCallback = playerComponent->m_dialogCallback.Get(resource->m_isolate);
        dialogCallback->Call(ctx, ctx->Global(), args.size(), args.data());
    });
}

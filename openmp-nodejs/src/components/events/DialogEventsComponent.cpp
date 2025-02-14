
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
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8dialogId  = v8::Integer::New(resource->m_isolate, dialogId);
        auto v8response  = v8::Integer::New(resource->m_isolate, response);
        auto v8listItem  = v8::Integer::New(resource->m_isolate, listItem);
        auto v8inputText = Utils::v8Str(inputText.data());

        resource->Emit("onDialogResponse", { v8objPlayer, v8dialogId, v8response, v8listItem, v8inputText });
    });
}

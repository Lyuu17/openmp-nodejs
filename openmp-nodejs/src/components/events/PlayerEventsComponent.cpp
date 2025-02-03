
#include "components/events/PlayerEventsComponent.hpp"
#include "components/PlayerComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

PlayerEventsComponent::PlayerEventsComponent(ICore* core)
    : m_core(core)
{
    m_core->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerTextDispatcher().addEventHandler(this);
}

PlayerEventsComponent::~PlayerEventsComponent()
{
    if (m_core)
    {
        m_core->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerTextDispatcher().removeEventHandler(this);
    }
}

void PlayerEventsComponent::freeExtension()
{
    delete this;
}

void PlayerEventsComponent::reset()
{
}

// =========== events

void PlayerEventsComponent::onIncomingConnection(IPlayer& player, StringView ipAddress, unsigned short port)
{
    player.addExtension(new PlayerComponent(m_core->getPlayers().get(player.getID())), true);

    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::String>  ip          = v8::String::NewFromUtf8(resource->m_isolate, ipAddress.data()).ToLocalChecked();
        v8::Local<v8::Integer> port_       = v8::Integer::New(resource->m_isolate, (int32_t)port);

        resource->Emit("onIncomingConnection", { v8objPlayer, ip, port_ });
    });
}

void PlayerEventsComponent::onPlayerConnect(IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerConnect", { v8objPlayer });
    });
}

void PlayerEventsComponent::onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Integer> reason_     = v8::Integer::New(resource->m_isolate, (int32_t)reason);

        resource->Emit("onPlayerDisconnect", { v8objPlayer, reason_ });
    });
}

void PlayerEventsComponent::onPlayerClientInit(IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerClientInit", { v8objPlayer });
    });
}

bool PlayerEventsComponent::onPlayerText(IPlayer& player, StringView message)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::String> message_    = v8::String::NewFromUtf8(resource->m_isolate, message.data()).ToLocalChecked();

        resource->Emit("onPlayerText", { cancellableEventObj, v8objPlayer, message_ });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return cancelled; // TODO
}

bool PlayerEventsComponent::onPlayerCommandText(IPlayer& player, StringView message)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::String> message_    = v8::String::NewFromUtf8(resource->m_isolate, message.data()).ToLocalChecked();

        resource->Emit("onPlayerCommandText", { cancellableEventObj, v8objPlayer, message_ });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled; // TODO
}

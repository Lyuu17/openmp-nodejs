
#include "components/events/PlayerEventsComponent.hpp"
#include "components/PlayerComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

PlayerEventsComponent::PlayerEventsComponent(ICore* core)
    : m_core(core)
{
    m_core->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerStreamDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerTextDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerShotDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerChangeDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerDamageDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerClickDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerCheckDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerUpdateDispatcher().addEventHandler(this);
}

PlayerEventsComponent::~PlayerEventsComponent()
{
    if (m_core)
    {
        m_core->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerStreamDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerTextDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerShotDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerChangeDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerDamageDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerClickDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerCheckDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerUpdateDispatcher().removeEventHandler(this);
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

    return cancelled;
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

    return !cancelled;
}

bool PlayerEventsComponent::onPlayerRequestSpawn(IPlayer& player) { return true; }

void PlayerEventsComponent::onPlayerSpawn(IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerSpawn", { v8objPlayer });
    });
}

void PlayerEventsComponent::onPlayerStreamIn(IPlayer& player, IPlayer& forPlayer)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer    = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objForPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(forPlayer));

        resource->Emit("onPlayerStreamIn", { v8objPlayer, v8objForPlayer });
    });
}

void PlayerEventsComponent::onPlayerStreamOut(IPlayer& player, IPlayer& forPlayer)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer    = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objForPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(forPlayer));

        resource->Emit("onPlayerStreamOut", { v8objPlayer, v8objForPlayer });
    });
}

bool PlayerEventsComponent::onPlayerShotMissed(IPlayer& player, const PlayerBulletData& bulletData) { return true; }
bool PlayerEventsComponent::onPlayerShotPlayer(IPlayer& player, IPlayer& target, const PlayerBulletData& bulletData) { return true; }
bool PlayerEventsComponent::onPlayerShotVehicle(IPlayer& player, IVehicle& target, const PlayerBulletData& bulletData) { return true; }
bool PlayerEventsComponent::onPlayerShotObject(IPlayer& player, IObject& target, const PlayerBulletData& bulletData) { return true; }
bool PlayerEventsComponent::onPlayerShotPlayerObject(IPlayer& player, IPlayerObject& target, const PlayerBulletData& bulletData) { return true; }

void PlayerEventsComponent::onPlayerScoreChange(IPlayer& player, int score)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Number> v8score     = v8::Number::New(resource->m_isolate, score);

        resource->Emit("onPlayerStreamOut", { v8objPlayer, v8score });
    });
}

void PlayerEventsComponent::onPlayerNameChange(IPlayer& player, StringView oldName)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerNameChange", { v8objPlayer, Utils::v8Str(oldName.data()) });
    });
}

void PlayerEventsComponent::onPlayerInteriorChange(IPlayer& player, unsigned newInterior, unsigned oldInterior)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer   = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Number> v8newInterior = v8::Number::New(resource->m_isolate, newInterior);
        v8::Local<v8::Number> v8oldInterior = v8::Number::New(resource->m_isolate, oldInterior);

        resource->Emit("onPlayerInteriorChange", { v8objPlayer, v8newInterior, v8oldInterior });
    });
}

void PlayerEventsComponent::onPlayerStateChange(IPlayer& player, PlayerState newState, PlayerState oldState)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Number> v8newState  = v8::Number::New(resource->m_isolate, newState);
        v8::Local<v8::Number> v8oldState  = v8::Number::New(resource->m_isolate, oldState);

        resource->Emit("onPlayerStateChange", { v8objPlayer, v8newState, v8oldState });
    });
}

void PlayerEventsComponent::onPlayerKeyStateChange(IPlayer& player, uint32_t newKeys, uint32_t oldKeys)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Number> v8newKeys   = v8::Number::New(resource->m_isolate, newKeys);
        v8::Local<v8::Number> v8oldKeys   = v8::Number::New(resource->m_isolate, oldKeys);

        resource->Emit("onPlayerKeyStateChange", { v8objPlayer, v8newKeys, v8oldKeys });
    });
}

void PlayerEventsComponent::onPlayerDeath(IPlayer& player, IPlayer* killer, int reason)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objKiller = resource->ObjectFromExtension(queryExtension<PlayerComponent>(killer));
        v8::Local<v8::Number> v8reason    = v8::Number::New(resource->m_isolate, reason);

        resource->Emit("onPlayerDeath", { v8objPlayer, v8objKiller, v8reason });
    });
}

void PlayerEventsComponent::onPlayerTakeDamage(IPlayer& player, IPlayer* from, float amount, unsigned weapon, BodyPart part) {}
void PlayerEventsComponent::onPlayerGiveDamage(IPlayer& player, IPlayer& to, float amount, unsigned weapon, BodyPart part) {}
void PlayerEventsComponent::onPlayerClickMap(IPlayer& player, Vector3 pos) {}
void PlayerEventsComponent::onPlayerClickPlayer(IPlayer& player, IPlayer& clicked, PlayerClickSource source) {}
void PlayerEventsComponent::onClientCheckResponse(IPlayer& player, int actionType, int address, int results) {}
bool PlayerEventsComponent::onPlayerUpdate(IPlayer& player, TimePoint now) { return true; }

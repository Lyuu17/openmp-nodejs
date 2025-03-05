
#include "events/PlayerEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"
#include "classes/VehicleExtension.hpp"

#include "NodejsComponent.hpp"
#include "Utils.hpp"

PlayerEventsExtension::PlayerEventsExtension(ICore* core, ResourceManager* resourceManager)
    : m_core(core)
    , m_resourceManager(resourceManager)
{
    m_core->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerSpawnDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerStreamDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerTextDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerShotDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerChangeDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerDamageDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerClickDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerCheckDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerUpdateDispatcher().addEventHandler(this);
}

PlayerEventsExtension::~PlayerEventsExtension()
{
    if (m_core)
    {
        m_core->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerSpawnDispatcher().removeEventHandler(this);
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

void PlayerEventsExtension::freeExtension()
{
    delete this;
}

void PlayerEventsExtension::reset()
{
}

// =========== events

void PlayerEventsExtension::onIncomingConnection(IPlayer& player, StringView ipAddress, unsigned short port)
{
    player.addExtension(new PlayerExtension(m_core->getPlayers().get(player.getID()), NodejsComponent::getInstance()->getResourceManager()), true);

    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::String>  ip          = v8::String::NewFromUtf8(resource->m_isolate, ipAddress.data()).ToLocalChecked();
        v8::Local<v8::Integer> port_       = v8::Integer::New(resource->m_isolate, (int32_t)port);

        resource->Emit("onIncomingConnection", { v8objPlayer, ip, port_ });
    });
}

void PlayerEventsExtension::onPlayerConnect(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerConnect", { v8objPlayer });
    });
}

void PlayerEventsExtension::onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Integer> reason_     = v8::Integer::New(resource->m_isolate, (int32_t)reason);

        resource->Emit("onPlayerDisconnect", { v8objPlayer, reason_ });
    });
}

void PlayerEventsExtension::onPlayerClientInit(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerClientInit", { v8objPlayer });
    });
}

bool PlayerEventsExtension::onPlayerText(IPlayer& player, StringView message)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::String> message_    = v8::String::NewFromUtf8(resource->m_isolate, message.data()).ToLocalChecked();

        resource->Emit("onPlayerText", { cancellableEventObj, v8objPlayer, message_ });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool PlayerEventsExtension::onPlayerCommandText(IPlayer& player, StringView message)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::String> message_    = v8::String::NewFromUtf8(resource->m_isolate, message.data()).ToLocalChecked();

        resource->Emit("onPlayerCommandText", { cancellableEventObj, v8objPlayer, message_ });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        if (!cancelled)
            cancelled = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return cancelled;
}

bool PlayerEventsExtension::onPlayerRequestSpawn(IPlayer& player)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerRequestSpawn", { cancellableEventObj, v8objPlayer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        if (!cancelled)
            cancelled = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

void PlayerEventsExtension::onPlayerSpawn(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerSpawn", { v8objPlayer });
    });
}

void PlayerEventsExtension::onPlayerStreamIn(IPlayer& player, IPlayer& forPlayer)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer    = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objForPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(forPlayer));

        resource->Emit("onPlayerStreamIn", { v8objPlayer, v8objForPlayer });
    });
}

void PlayerEventsExtension::onPlayerStreamOut(IPlayer& player, IPlayer& forPlayer)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer    = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objForPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(forPlayer));

        resource->Emit("onPlayerStreamOut", { v8objPlayer, v8objForPlayer });
    });
}

bool PlayerEventsExtension::onPlayerShotMissed(IPlayer& player, const PlayerBulletData& bulletData)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer     = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objBulletData = Utils::v8PlayerBulletData(bulletData);

        resource->Emit("onPlayerShotMissed", { cancellableEventObj, v8objPlayer, v8objBulletData });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        if (!cancelled)
            cancelled = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool PlayerEventsExtension::onPlayerShotPlayer(IPlayer& player, IPlayer& target, const PlayerBulletData& bulletData)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer     = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objTarget     = resource->ObjectFromExtension(queryExtension<PlayerExtension>(target));
        auto v8objBulletData = Utils::v8PlayerBulletData(bulletData);

        resource->Emit("onPlayerShotPlayer", { cancellableEventObj, v8objPlayer, v8objTarget, v8objBulletData });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        if (!cancelled)
            cancelled = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool PlayerEventsExtension::onPlayerShotVehicle(IPlayer& player, IVehicle& target, const PlayerBulletData& bulletData)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer     = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objTarget     = resource->ObjectFromExtension(queryExtension<VehicleExtension>(target));
        auto v8objBulletData = Utils::v8PlayerBulletData(bulletData);

        resource->Emit("onPlayerShotVehicle", { cancellableEventObj, v8objPlayer, v8objTarget, v8objBulletData });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        if (!cancelled)
            cancelled = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool PlayerEventsExtension::onPlayerShotObject(IPlayer& player, IObject& target, const PlayerBulletData& bulletData) { return true; }
bool PlayerEventsExtension::onPlayerShotPlayerObject(IPlayer& player, IPlayerObject& target, const PlayerBulletData& bulletData) { return true; }

void PlayerEventsExtension::onPlayerScoreChange(IPlayer& player, int score)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Number> v8score     = v8::Number::New(resource->m_isolate, score);

        resource->Emit("onPlayerScoreChange", { v8objPlayer, v8score });
    });
}

void PlayerEventsExtension::onPlayerNameChange(IPlayer& player, StringView oldName)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerNameChange", { v8objPlayer, Utils::v8Str(oldName.data()) });
    });
}

void PlayerEventsExtension::onPlayerInteriorChange(IPlayer& player, unsigned newInterior, unsigned oldInterior)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer   = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Number> v8newInterior = v8::Number::New(resource->m_isolate, newInterior);
        v8::Local<v8::Number> v8oldInterior = v8::Number::New(resource->m_isolate, oldInterior);

        resource->Emit("onPlayerInteriorChange", { v8objPlayer, v8newInterior, v8oldInterior });
    });
}

void PlayerEventsExtension::onPlayerStateChange(IPlayer& player, PlayerState newState, PlayerState oldState)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Number> v8newState  = v8::Number::New(resource->m_isolate, newState);
        v8::Local<v8::Number> v8oldState  = v8::Number::New(resource->m_isolate, oldState);

        resource->Emit("onPlayerStateChange", { v8objPlayer, v8newState, v8oldState });
    });
}

void PlayerEventsExtension::onPlayerKeyStateChange(IPlayer& player, uint32_t newKeys, uint32_t oldKeys)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Number> v8newKeys   = v8::Number::New(resource->m_isolate, newKeys);
        v8::Local<v8::Number> v8oldKeys   = v8::Number::New(resource->m_isolate, oldKeys);

        resource->Emit("onPlayerKeyStateChange", { v8objPlayer, v8newKeys, v8oldKeys });
    });
}

void PlayerEventsExtension::onPlayerDeath(IPlayer& player, IPlayer* killer, int reason)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Value>  v8objKiller = killer ? resource->ObjectFromExtension(queryExtension<PlayerExtension>(killer)).As<v8::Value>() : v8::Null(resource->m_isolate).As<v8::Value>();
        v8::Local<v8::Number> v8reason    = v8::Number::New(resource->m_isolate, reason);

        resource->Emit("onPlayerDeath", { v8objPlayer, v8objKiller, v8reason });
    });
}

void PlayerEventsExtension::onPlayerTakeDamage(IPlayer& player, IPlayer* from, float amount, unsigned weapon, BodyPart part)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objFrom   = from ? resource->ObjectFromExtension(queryExtension<PlayerExtension>(from)).As<v8::Value>() : v8::Null(resource->m_isolate).As<v8::Value>();
        auto v8amount    = v8::Number::New(resource->m_isolate, amount);
        auto v8weapon    = v8::Integer::New(resource->m_isolate, weapon);
        auto v8part      = v8::Integer::New(resource->m_isolate, part);

        resource->Emit("onPlayerTakeDamage", { v8objPlayer, v8objFrom, v8amount, v8weapon, v8part });
    });
}

void PlayerEventsExtension::onPlayerGiveDamage(IPlayer& player, IPlayer& to, float amount, unsigned weapon, BodyPart part)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objTo     = resource->ObjectFromExtension(queryExtension<PlayerExtension>(to));
        auto v8amount    = v8::Number::New(resource->m_isolate, amount);
        auto v8weapon    = v8::Integer::New(resource->m_isolate, weapon);
        auto v8part      = v8::Integer::New(resource->m_isolate, part);

        resource->Emit("onPlayerGiveDamage", { v8objPlayer, v8objTo, v8amount, v8weapon, v8part });
    });
}

void PlayerEventsExtension::onPlayerClickMap(IPlayer& player, Vector3 pos)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8position  = Utils::v8Vector3(pos);

        resource->Emit("onPlayerClickMap", { v8objPlayer, v8position });
    });
}

void PlayerEventsExtension::onPlayerClickPlayer(IPlayer& player, IPlayer& clicked, PlayerClickSource source) {}
void PlayerEventsExtension::onClientCheckResponse(IPlayer& player, int actionType, int address, int results) {}

bool PlayerEventsExtension::onPlayerUpdate(IPlayer& player, TimePoint now)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8time      = v8::Integer::New(resource->m_isolate, std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count());

        resource->Emit("onPlayerUpdate", { cancellableEventObj, v8objPlayer, v8time });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        if (!cancelled)
            cancelled = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

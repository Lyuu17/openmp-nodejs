
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <sdk.hpp>

struct PlayerEventsComponent final : public IExtension,
                                     public PlayerConnectEventHandler,
                                     public PlayerSpawnEventHandler,
                                     public PlayerStreamEventHandler,
                                     public PlayerTextEventHandler,
                                     public PlayerShotEventHandler,
                                     public PlayerChangeEventHandler,
                                     public PlayerDamageEventHandler,
                                     public PlayerClickEventHandler,
                                     public PlayerCheckEventHandler,
                                     public PlayerUpdateEventHandler
{
    PROVIDE_EXT_UID(0x7069E1695D3C87EB);

    ICore*           m_core            = nullptr;
    ResourceManager* m_resourceManager = nullptr;

public:
    PlayerEventsComponent(ICore*, ResourceManager*);
    ~PlayerEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onIncomingConnection(IPlayer& player, StringView ipAddress, unsigned short port) override;
    void onPlayerConnect(IPlayer& player) override;
    void onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason) override;
    void onPlayerClientInit(IPlayer& player) override;
    bool onPlayerText(IPlayer& player, StringView message) override;
    bool onPlayerCommandText(IPlayer& player, StringView message) override;
    bool onPlayerRequestSpawn(IPlayer& player) override;
    void onPlayerSpawn(IPlayer& player) override;
    void onPlayerStreamIn(IPlayer& player, IPlayer& forPlayer) override;
    void onPlayerStreamOut(IPlayer& player, IPlayer& forPlayer) override;
    bool onPlayerShotMissed(IPlayer& player, const PlayerBulletData& bulletData) override;
    bool onPlayerShotPlayer(IPlayer& player, IPlayer& target, const PlayerBulletData& bulletData) override;
    bool onPlayerShotVehicle(IPlayer& player, IVehicle& target, const PlayerBulletData& bulletData) override;
    bool onPlayerShotObject(IPlayer& player, IObject& target, const PlayerBulletData& bulletData) override;
    bool onPlayerShotPlayerObject(IPlayer& player, IPlayerObject& target, const PlayerBulletData& bulletData) override;
    void onPlayerScoreChange(IPlayer& player, int score) override;
    void onPlayerNameChange(IPlayer& player, StringView oldName) override;
    void onPlayerInteriorChange(IPlayer& player, unsigned newInterior, unsigned oldInterior) override;
    void onPlayerStateChange(IPlayer& player, PlayerState newState, PlayerState oldState) override;
    void onPlayerKeyStateChange(IPlayer& player, uint32_t newKeys, uint32_t oldKeys) override;
    void onPlayerDeath(IPlayer& player, IPlayer* killer, int reason) override;
    void onPlayerTakeDamage(IPlayer& player, IPlayer* from, float amount, unsigned weapon, BodyPart part) override;
    void onPlayerGiveDamage(IPlayer& player, IPlayer& to, float amount, unsigned weapon, BodyPart part) override;
    void onPlayerClickMap(IPlayer& player, Vector3 pos) override;
    void onPlayerClickPlayer(IPlayer& player, IPlayer& clicked, PlayerClickSource source) override;
    void onClientCheckResponse(IPlayer& player, int actionType, int address, int results) override;
    bool onPlayerUpdate(IPlayer& player, TimePoint now) override;
};

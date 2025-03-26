
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <StreamerComponent.hpp>

struct StreamerModuleEvents final : public IExtension,
                                    public streamer::StreamerEventHandler
{
    PROVIDE_EXT_UID(0xB7E2BC45C6083233);

    ICore*                 m_core            = nullptr;
    IOmpStreamerComponent* m_streamer        = nullptr;
    ResourceManager*       m_resourceManager = nullptr;

public:
    StreamerModuleEvents(ICore*, IOmpStreamerComponent* streamer, ResourceManager* resourceManager);
    ~StreamerModuleEvents();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onDynamicObjectMoved(const std::shared_ptr<streamer::IObject>& object) override;
    void onPlayerEditDynamicObject(IPlayer& player, const std::shared_ptr<streamer::IObject>& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation) override;
    void onPlayerSelectDynamicObject(IPlayer& player, const std::shared_ptr<streamer::IObject>& object) override;
    void onPlayerShootDynamicObject(IPlayer& player, const std::shared_ptr<streamer::IObject>& object, const PlayerBulletData& bulletData) override;
    void onPlayerPickUpDynamicPickup(IPlayer& player, const std::shared_ptr<streamer::IPickup>& pickup) override;
    void onPlayerEnterDynamicCP(IPlayer& player, const std::shared_ptr<streamer::ICheckpoint>& cp) override;
    void onPlayerLeaveDynamicCP(IPlayer& player, const std::shared_ptr<streamer::ICheckpoint>& cp) override;
    void onPlayerEnterDynamicRaceCP(IPlayer& player, const std::shared_ptr<streamer::IRaceCheckpoint>& cp) override;
    void onPlayerLeaveDynamicRaceCP(IPlayer& player, const std::shared_ptr<streamer::IRaceCheckpoint>& cp) override;
    void onPlayerEnterDynamicArea(IPlayer& player, const std::shared_ptr<streamer::IArea>& area) override;
    void onPlayerLeaveDynamicArea(IPlayer& player, const std::shared_ptr<streamer::IArea>& area) override;
    void onPlayerGiveDamageDynamicActor(IPlayer& player, const std::shared_ptr<streamer::IActor>& actor, float amount, unsigned weapon, BodyPart part) override;
    void onDynamicActorStreamIn(IPlayer& player, const std::shared_ptr<streamer::IActor>& actor) override;
    void onDynamicActorStreamOut(IPlayer& player, const std::shared_ptr<streamer::IActor>& actor) override;
    void onItemStreamIn(IPlayer& player, int itemId, StreamerItemType type) override;
    void onItemStreamOut(IPlayer& player, int itemId, StreamerItemType type) override;
};

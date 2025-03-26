
#include "modules/streamer/StreamerModuleEvents.hpp"
#include "modules/streamer/StreamerObjectExtension.hpp"
#include "classes/PlayerExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

StreamerModuleEvents::StreamerModuleEvents(ICore* core, IOmpStreamerComponent* streamer, ResourceManager* resourceManager)
    : m_core(core)
    , m_streamer(streamer)
    , m_resourceManager(resourceManager)
{
    if (streamer)
    {
        streamer->getEventDispatcher().addEventHandler(this);
    }
}

StreamerModuleEvents::~StreamerModuleEvents()
{
    if (m_core)
    {
    }
}

void StreamerModuleEvents::freeExtension()
{
    delete this;
}

void StreamerModuleEvents::reset()
{
}

// =========== events

void StreamerModuleEvents::onDynamicObjectMoved(const std::shared_ptr<streamer::IObject>& object)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto obj = resource->ObjectFromExtension(queryExtension<StreamerObjectExtension>(object.get()));

        resource->Emit("onDynamicObjectMoved", { obj });
    });
}

void StreamerModuleEvents::onPlayerEditDynamicObject(IPlayer& player, const std::shared_ptr<streamer::IObject>& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto _player   = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto obj       = resource->ObjectFromExtension(queryExtension<StreamerObjectExtension>(object.get()));
        auto _response = v8::Number::New(v8::Isolate::GetCurrent(), response);
        auto _offset   = Utils::v8Vector3(offset);
        auto _rotation = Utils::v8Vector3(offset);

        resource->Emit("onPlayerEditDynamicObject", { _player, obj, _response, _offset, _rotation });
    });
}

void StreamerModuleEvents::onPlayerSelectDynamicObject(IPlayer& player, const std::shared_ptr<streamer::IObject>& object)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto _player = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto obj     = resource->ObjectFromExtension(queryExtension<StreamerObjectExtension>(object.get()));

        resource->Emit("onPlayerSelectDynamicObject", { _player, obj });
    });
}

void StreamerModuleEvents::onPlayerShootDynamicObject(IPlayer& player, const std::shared_ptr<streamer::IObject>& object, const PlayerBulletData& bulletData)
{
}

void StreamerModuleEvents::onPlayerPickUpDynamicPickup(IPlayer& player, const std::shared_ptr<streamer::IPickup>& pickup)
{}

void StreamerModuleEvents::onPlayerEnterDynamicCP(IPlayer& player, const std::shared_ptr<streamer::ICheckpoint>& cp)
{}

void StreamerModuleEvents::onPlayerLeaveDynamicCP(IPlayer& player, const std::shared_ptr<streamer::ICheckpoint>& cp)
{}

void StreamerModuleEvents::onPlayerEnterDynamicRaceCP(IPlayer& player, const std::shared_ptr<streamer::IRaceCheckpoint>& cp)
{}

void StreamerModuleEvents::onPlayerLeaveDynamicRaceCP(IPlayer& player, const std::shared_ptr<streamer::IRaceCheckpoint>& cp)
{}

void StreamerModuleEvents::onPlayerEnterDynamicArea(IPlayer& player, const std::shared_ptr<streamer::IArea>& area)
{}

void StreamerModuleEvents::onPlayerLeaveDynamicArea(IPlayer& player, const std::shared_ptr<streamer::IArea>& area)
{}

void StreamerModuleEvents::onPlayerGiveDamageDynamicActor(IPlayer& player, const std::shared_ptr<streamer::IActor>& actor, float amount, unsigned weapon, BodyPart part)
{}

void StreamerModuleEvents::onDynamicActorStreamIn(IPlayer& player, const std::shared_ptr<streamer::IActor>& actor)
{}

void StreamerModuleEvents::onDynamicActorStreamOut(IPlayer& player, const std::shared_ptr<streamer::IActor>& actor)
{}

void StreamerModuleEvents::onItemStreamIn(IPlayer& player, int itemId, StreamerItemType type)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto _player = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto _itemId = v8::Number::New(v8::Isolate::GetCurrent(), itemId);
        auto _type   = v8::Number::New(v8::Isolate::GetCurrent(), type);

        resource->Emit("onItemStreamIn", { _player, _itemId, _type });
    });
}

void StreamerModuleEvents::onItemStreamOut(IPlayer& player, int itemId, StreamerItemType type)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto _player = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto _itemId = v8::Number::New(v8::Isolate::GetCurrent(), itemId);
        auto _type   = v8::Number::New(v8::Isolate::GetCurrent(), type);

        resource->Emit("onItemStreamOut", { _player, _itemId, _type });
    });
}

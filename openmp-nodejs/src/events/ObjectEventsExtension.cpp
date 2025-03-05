
#include "events/ObjectEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"
#include "classes/ObjectExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

ObjectEventsExtension::ObjectEventsExtension(ICore* core, IObjectsComponent* object, ResourceManager* resourceManager)
    : m_core(core)
    , m_objects(object)
    , m_resourceManager(resourceManager)
{
    if (m_objects)
    {
        m_objects->getEventDispatcher().addEventHandler(this);
    }
}

ObjectEventsExtension::~ObjectEventsExtension()
{
}

void ObjectEventsExtension::freeExtension()
{
    delete this;
}

void ObjectEventsExtension::reset()
{
}

// =========== events

void ObjectEventsExtension::onMoved(IObject& object)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objMoved = resource->ObjectFromExtension(queryExtension<ObjectExtension>(object));

        resource->Emit("onObjectMoved", { v8objMoved });
    });
}

void ObjectEventsExtension::onPlayerObjectMoved(IPlayer& player, IPlayerObject& object)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objMoved  = resource->ObjectFromExtension(queryExtension<ObjectExtension>(object));

        resource->Emit("onPlayerObjectMoved", { v8objPlayer, v8objMoved });
    });
}

void ObjectEventsExtension::onObjectSelected(IPlayer& player, IObject& object, int model, Vector3 position)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectExtension>(object));
        auto v8model     = v8::Integer::New(resource->m_isolate, model);
        auto v8position  = Utils::v8Vector3(position);

        resource->Emit("onObjectSelected", { v8objPlayer, v8objObject, v8model, v8position });
    });
}

void ObjectEventsExtension::onPlayerObjectSelected(IPlayer& player, IPlayerObject& object, int model, Vector3 position)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectExtension>(object));
        auto v8model     = v8::Integer::New(resource->m_isolate, model);
        auto v8position  = Utils::v8Vector3(position);

        resource->Emit("onPlayerObjectSelected", { v8objPlayer, v8objObject, v8model, v8position });
    });
}

void ObjectEventsExtension::onObjectEdited(IPlayer& player, IObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectExtension>(object));
        auto v8response  = v8::Integer::New(resource->m_isolate, response);
        auto v8offset    = Utils::v8Vector3(offset);
        auto v8rotation  = Utils::v8Vector3(rotation);

        resource->Emit("onObjectEdited", { v8objPlayer, v8objObject, v8response, v8offset, v8rotation });
    });
}

void ObjectEventsExtension::onPlayerObjectEdited(IPlayer& player, IPlayerObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectExtension>(object));
        auto v8response  = v8::Integer::New(resource->m_isolate, response);
        auto v8offset    = Utils::v8Vector3(offset);
        auto v8rotation  = Utils::v8Vector3(rotation);

        resource->Emit("onPlayerObjectEdited", { v8objPlayer, v8objObject, v8response, v8offset, v8rotation });
    });
}

void ObjectEventsExtension::onPlayerAttachedObjectEdited(IPlayer& player, int index, bool saved, const ObjectAttachmentSlotData& data)
{
}

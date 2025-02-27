
#include "components/events/ObjectEventsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/ObjectComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

ObjectEventsComponent::ObjectEventsComponent(ICore* core, IObjectsComponent* object, ResourceManager* resourceManager)
    : m_core(core)
    , m_objects(object)
    , m_resourceManager(resourceManager)
{
    if (m_objects)
    {
        m_objects->getEventDispatcher().addEventHandler(this);
    }
}

ObjectEventsComponent::~ObjectEventsComponent()
{
}

void ObjectEventsComponent::freeExtension()
{
    delete this;
}

void ObjectEventsComponent::reset()
{
}

// =========== events

void ObjectEventsComponent::onMoved(IObject& object)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objMoved = resource->ObjectFromExtension(queryExtension<ObjectComponent>(object));

        resource->Emit("onObjectMoved", { v8objMoved });
    });
}

void ObjectEventsComponent::onPlayerObjectMoved(IPlayer& player, IPlayerObject& object)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objMoved  = resource->ObjectFromExtension(queryExtension<ObjectComponent>(object));

        resource->Emit("onPlayerObjectMoved", { v8objPlayer, v8objMoved });
    });
}

void ObjectEventsComponent::onObjectSelected(IPlayer& player, IObject& object, int model, Vector3 position)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectComponent>(object));
        auto v8model     = v8::Integer::New(resource->m_isolate, model);
        auto v8position  = Utils::v8Vector3(position);

        resource->Emit("onObjectSelected", { v8objPlayer, v8objObject, v8model, v8position });
    });
}

void ObjectEventsComponent::onPlayerObjectSelected(IPlayer& player, IPlayerObject& object, int model, Vector3 position)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectComponent>(object));
        auto v8model     = v8::Integer::New(resource->m_isolate, model);
        auto v8position  = Utils::v8Vector3(position);

        resource->Emit("onPlayerObjectSelected", { v8objPlayer, v8objObject, v8model, v8position });
    });
}

void ObjectEventsComponent::onObjectEdited(IPlayer& player, IObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectComponent>(object));
        auto v8response  = v8::Integer::New(resource->m_isolate, response);
        auto v8offset    = Utils::v8Vector3(offset);
        auto v8rotation  = Utils::v8Vector3(rotation);

        resource->Emit("onObjectEdited", { v8objPlayer, v8objObject, v8response, v8offset, v8rotation });
    });
}

void ObjectEventsComponent::onPlayerObjectEdited(IPlayer& player, IPlayerObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8objObject = resource->ObjectFromExtension(queryExtension<ObjectComponent>(object));
        auto v8response  = v8::Integer::New(resource->m_isolate, response);
        auto v8offset    = Utils::v8Vector3(offset);
        auto v8rotation  = Utils::v8Vector3(rotation);

        resource->Emit("onPlayerObjectEdited", { v8objPlayer, v8objObject, v8response, v8offset, v8rotation });
    });
}

void ObjectEventsComponent::onPlayerAttachedObjectEdited(IPlayer& player, int index, bool saved, const ObjectAttachmentSlotData& data)
{
}

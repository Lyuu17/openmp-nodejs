
#include <sdk.hpp>

#include "classes/PickupExtension.hpp"

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_PICKUP_HAS_COMPONENT(pickup, component)                                                           \
    if (!queryExtension<component>(pickup))                                                                      \
    {                                                                                                            \
        pickup->addExtension(new component(pickup, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<component>(pickup));                                                               \
    }

PickupExtension::PickupExtension(IPickup* pickup, ResourceManager* resourceManager)
    : m_pickup(pickup)
    , m_resourceManager(resourceManager)
{
}

PickupExtension::~PickupExtension()
{
}

void PickupExtension::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void PickupExtension::reset()
{
}

// ====================== methods ======================

void PickupExtension::destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    NodejsComponent::getInstance()->getPickups()->release(pickupComponent->m_pickup->getID());
}

void PickupExtension::setPickupHiddenForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    auto v8hidden = Utils::GetBooleanFromV8Value(info[1]);
    if (!v8hidden.has_value())
        return;

    pickupComponent->m_pickup->setPickupHiddenForPlayer(*player, v8hidden.value());
}

void PickupExtension::isPickupHiddenForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    info.GetReturnValue().Set(pickupComponent->m_pickup->isPickupHiddenForPlayer(*player));
}

// ====================== accessors ======================

void PickupExtension::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), pickupComponent->m_pickup->getID()));
}

void PickupExtension::getType(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), (int)pickupComponent->m_pickup->getType()));
}

void PickupExtension::setType(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    pickupComponent->m_pickup->setType((PickupType)v8int.value());
}

void PickupExtension::getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), pickupComponent->m_pickup->getModel()));
}

void PickupExtension::setModel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    pickupComponent->m_pickup->setModel(v8int.value());
}

void PickupExtension::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(pickupComponent->m_pickup->getPosition()));
}

void PickupExtension::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    pickupComponent->m_pickup->setPositionNoUpdate(v8vec3.value());
}

void PickupExtension::getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), pickupComponent->m_pickup->getVirtualWorld()));
}

void PickupExtension::setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto pickupComponent = (PickupExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PickupExtension>(info.GetIsolate(), pickupComponent)) return;

    auto v8value = Utils::GetIntegerFromV8Value(value);
    if (!v8value.has_value())
        return;

    pickupComponent->m_pickup->setVirtualWorld(v8value.value());
}

v8::Local<v8::Object> PickupExtension::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("destroy", destroy);
    SET_FUNCTION("setPickupHiddenForPlayer", setPickupHiddenForPlayer);
    SET_FUNCTION("isPickupHiddenForPlayer", isPickupHiddenForPlayer);

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("type", getType, setType);
    SET_ACCESSOR_WITH_SETTER("model", getModel, setModel);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("virtualWorld", getVirtualWorld, setVirtualWorld);

    return v8obj;
}

void PickupExtension::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("getPickup"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8int = Utils::GetIntegerFromV8Value(info[0]);
        if (!v8int.has_value())
            return;

        auto pickup = NodejsComponent::getInstance()->getPickups()->get(v8int.value());
        if (!pickup)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        ENSURE_PICKUP_HAS_COMPONENT(pickup, PickupExtension);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<PickupExtension>(pickup)));
    }, v8::External::New(isolate, resource))
                                                                   .ToLocalChecked());

    context->Global()->Set(context, Utils::v8Str("createPickup"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8model        = Utils::GetIntegerFromV8Value(info[0]);
        auto v8pickupType   = Utils::GetIntegerFromV8Value(info[1]);
        auto v8pos          = Utils::vector3V8(info[2]);
        auto v8virtualworld = Utils::GetIntegerFromV8Value(info[3]);
        auto v8isStatic     = Utils::GetBooleanFromV8Value(info[4]);

        if (!v8model.has_value() || !v8pickupType.has_value() || !v8pos.has_value() || !v8virtualworld.has_value() || !v8isStatic.has_value())
            return;

        auto pickup = NodejsComponent::getInstance()->getPickups()->create(v8model.value(), v8pickupType.value(), v8pos.value(), v8virtualworld.value(), v8isStatic.value());
        if (pickup == nullptr)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        ENSURE_PICKUP_HAS_COMPONENT(pickup, PickupExtension);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<PickupExtension>(pickup)));
    }, v8::External::New(isolate, resource))
                                                                      .ToLocalChecked());
}

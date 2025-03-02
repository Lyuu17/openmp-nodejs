
#include <sdk.hpp>

#include "components/ObjectComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_HAS_EXTENSION(ext, player, iext)                                                                  \
    if (!queryExtension<iext>(ext))                                                                              \
    {                                                                                                            \
        ext->addExtension(new iext(object, player, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<iext>(object));                                                                    \
    }

ObjectComponent::ObjectComponent(IBaseObject* object, IPlayer* player, ResourceManager* resourceManager)
    : m_object(object)
    , m_player(player)
    , m_resourceManager(resourceManager)
{
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void ObjectComponent::reset()
{
}

// ====================== methods ======================

void ObjectComponent::destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    NodejsComponent::getInstance()->getObjects()->release(objectComponent->m_object->getID());
}

void ObjectComponent::move(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto targetPos = Utils::vector3V8(info[0]);
    auto targetRot = Utils::vector3V8(info[1]);
    auto speed     = Utils::GetDoubleFromV8Value(info[2]);

    if (!targetPos.has_value() || !targetRot.has_value() || !speed.has_value())
        return;

    objectComponent->m_object->move({ targetPos.value(), targetRot.value(), (float)speed.value() });
}

void ObjectComponent::stop(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    objectComponent->m_object->stop();
}

void ObjectComponent::isStreamedInForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto object = dynamic_cast<IObject*>(objectComponent->m_object);
    if (!object) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    info.GetReturnValue().Set(object->isStreamedInForPlayer(*player));
}

void ObjectComponent::setStreamedForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto object = dynamic_cast<IObject*>(objectComponent->m_object);
    if (!object) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    auto v8stream = Utils::GetBooleanFromV8Value(info[1]);
    if (!v8stream.has_value())
        return;

    if (v8stream.value())
        object->streamInForPlayer(*player);
    else
        object->streamOutForPlayer(*player);
}

// ====================== accessors ======================

void ObjectComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getID());
}

void ObjectComponent::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(objectComponent->m_object->getPosition()));
}

void ObjectComponent::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    objectComponent->m_object->setPosition(v8vec3.value());
}

void ObjectComponent::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(Utils::v8Quat(objectComponent->m_object->getRotation()));
}

void ObjectComponent::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto v8quat = Utils::quatV8(value);
    if (!v8quat.has_value())
        return;

    objectComponent->m_object->setRotation(v8quat.value());
}

void ObjectComponent::getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getVirtualWorld());
}

void ObjectComponent::setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value())
        return;

    objectComponent->m_object->setVirtualWorld(v.value());
}

void ObjectComponent::getDrawDistance(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getDrawDistance());
}

void ObjectComponent::setDrawDistance(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value())
        return;

    objectComponent->m_object->setDrawDistance(v.value());
}

void ObjectComponent::getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getModel());
}

void ObjectComponent::setModel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value())
        return;

    objectComponent->m_object->setModel(v.value());
}

void ObjectComponent::getAlwaysStreamedIn(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto object = dynamic_cast<IObject*>(objectComponent->m_object);
    if (!object) return;

    info.GetReturnValue().Set(object->isAlwaysStreamedIn());
}

void ObjectComponent::setAlwaysStreamedIn(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectComponent>(info.GetIsolate(), objectComponent)) return;

    auto object = dynamic_cast<IObject*>(objectComponent->m_object);
    if (!object) return;

    auto v = Utils::GetBooleanFromV8Value(value);
    if (!v.has_value())
        return;

    object->setAlwaysStreamedIn(v.value());
}

v8::Local<v8::Object> ObjectComponent::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("destroy", destroy);
    SET_FUNCTION("move", move);
    SET_FUNCTION("stop", stop);

    if (dynamic_cast<IObject*>(m_object))
    {
        SET_FUNCTION("isStreamedInForPlayer", isStreamedInForPlayer);
        SET_FUNCTION("setStreamedForPlayer", setStreamedForPlayer);
    }

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("rotation", getRotation, setRotation);
    SET_ACCESSOR_WITH_SETTER("virtualWorld", getVirtualWorld, setVirtualWorld);
    SET_ACCESSOR_WITH_SETTER("drawDistance", getDrawDistance, setDrawDistance);
    SET_ACCESSOR_WITH_SETTER("model", getModel, setModel);

    if (dynamic_cast<IObject*>(m_object))
    {
        SET_ACCESSOR_WITH_SETTER("alwaysStreamedIn", getAlwaysStreamedIn, setAlwaysStreamedIn);
    }

    return v8obj;
}

void ObjectComponent::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("createObject"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8model        = Utils::GetIntegerFromV8Value(info[0]);
        auto v8pos          = Utils::vector3V8(info[1]);
        auto v8rot          = Utils::vector3V8(info[2]);
        auto v8drawDistance = Utils::GetDoubleFromV8Value(info[3]);
        auto v8playerId     = Utils::GetIdFromV8Object(info[4]);
        if (!v8model.has_value() || !v8pos.has_value() || !v8rot.has_value())
            return;

        IPlayer*     player = nullptr;
        IBaseObject* object = nullptr;

        if (v8playerId.has_value())
        {
            player = NodejsComponent::getInstance()->getCore()->getPlayers().get(v8playerId.value());
            if (!player)
                return;

            auto playerObjectData = queryExtension<IPlayerObjectData>(player);
            if (!playerObjectData)
            {
                info.GetReturnValue().SetUndefined();
                return;
            }

            object = playerObjectData->create(v8model.value(), v8pos.value(), v8rot.value(), v8drawDistance.value_or(0.0f));

            ENSURE_HAS_EXTENSION(object, player, ObjectComponent);
        }
        else
        {
            object = NodejsComponent::getInstance()->getObjects()->create(v8model.value(), v8pos.value(), v8rot.value(), v8drawDistance.value_or(0.0f));

            ENSURE_HAS_EXTENSION(object, nullptr, ObjectComponent);
        }

        if (!object)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<ObjectComponent>(object)));
    }, v8::External::New(isolate, resource))
                                                                      .ToLocalChecked());

    context->Global()->Set(context, Utils::v8Str("getObject"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8id = Utils::GetIntegerFromV8Value(info[0]);
        if (!v8id.has_value())
            return;

        IPlayer*     player = nullptr;
        IBaseObject* object = nullptr;

        auto playerId = Utils::GetIdFromV8Object(info[1]);
        if (playerId.has_value())
        {
            player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
            if (!player)
                return;

            auto playerObjectData = queryExtension<IPlayerObjectData>(player);
            if (!playerObjectData)
            {
                info.GetReturnValue().SetUndefined();
                return;
            }

            object = playerObjectData->get(v8id.value());

            if (object) ENSURE_HAS_EXTENSION(object, player, ObjectComponent);
        }
        else
        {
            object = NodejsComponent::getInstance()->getObjects()->get(v8id.value());

            if (object) ENSURE_HAS_EXTENSION(object, nullptr, ObjectComponent);
        }

        if (!object)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<ObjectComponent>(object)));
    }, v8::External::New(isolate, resource))
                                                                   .ToLocalChecked());
}

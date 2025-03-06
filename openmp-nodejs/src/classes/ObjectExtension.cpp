
#include <sdk.hpp>

#include "classes/ObjectExtension.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_HAS_EXTENSION(ext, player, iext)                                                                  \
    if (!queryExtension<iext>(ext))                                                                              \
    {                                                                                                            \
        ext->addExtension(new iext(object, player, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<iext>(object));                                                                    \
    }

ObjectExtension::ObjectExtension(IBaseObject* object, IPlayer* player, ResourceManager* resourceManager)
    : m_object(object)
    , m_player(player)
    , m_resourceManager(resourceManager)
{
}

ObjectExtension::~ObjectExtension()
{
}

void ObjectExtension::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void ObjectExtension::reset()
{
}

// ====================== methods ======================

void ObjectExtension::destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    NodejsComponent::getInstance()->getObjects()->release(objectComponent->m_object->getID());
}

void ObjectExtension::move(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    auto targetPos = Utils::vector3V8(info[0]);
    auto targetRot = Utils::vector3V8(info[1]);
    auto speed     = Utils::GetDoubleFromV8Value(info[2]);

    if (!targetPos.has_value() || !targetRot.has_value() || !speed.has_value())
        return;

    objectComponent->m_object->move({ targetPos.value(), targetRot.value(), (float)speed.value() });
}

void ObjectExtension::stop(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    objectComponent->m_object->stop();
}

// ====================== accessors ======================

void ObjectExtension::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getID());
}

void ObjectExtension::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(objectComponent->m_object->getPosition()));
}

void ObjectExtension::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    objectComponent->m_object->setPosition(v8vec3.value());
}

void ObjectExtension::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(Utils::v8Quat(objectComponent->m_object->getRotation()));
}

void ObjectExtension::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    auto v8quat = Utils::quatV8(value);
    if (!v8quat.has_value())
        return;

    objectComponent->m_object->setRotation(v8quat.value());
}

void ObjectExtension::getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getVirtualWorld());
}

void ObjectExtension::setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value())
        return;

    objectComponent->m_object->setVirtualWorld(v.value());
}

void ObjectExtension::getDrawDistance(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getDrawDistance());
}

void ObjectExtension::setDrawDistance(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value())
        return;

    objectComponent->m_object->setDrawDistance(v.value());
}

void ObjectExtension::getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    info.GetReturnValue().Set(objectComponent->m_object->getModel());
}

void ObjectExtension::setModel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto objectComponent = (ObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<ObjectExtension>(info.GetIsolate(), objectComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value())
        return;

    objectComponent->m_object->setModel(v.value());
}

v8::Local<v8::Object> ObjectExtension::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("destroy", destroy);
    SET_FUNCTION("move", move);
    SET_FUNCTION("stop", stop);

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("rotation", getRotation, setRotation);
    SET_ACCESSOR_WITH_SETTER("virtualWorld", getVirtualWorld, setVirtualWorld);
    SET_ACCESSOR_WITH_SETTER("drawDistance", getDrawDistance, setDrawDistance);
    SET_ACCESSOR_WITH_SETTER("model", getModel, setModel);

    return v8obj;
}

void ObjectExtension::InitFunctions(Resource* resource)
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

            if (object) ENSURE_HAS_EXTENSION(object, player, ObjectExtension);
        }
        else
        {
            object = NodejsComponent::getInstance()->getObjects()->create(v8model.value(), v8pos.value(), v8rot.value(), v8drawDistance.value_or(0.0f));

            if (object) ENSURE_HAS_EXTENSION(object, nullptr, ObjectExtension);
        }

        if (!object)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<ObjectExtension>(object)));
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

            if (object) ENSURE_HAS_EXTENSION(object, player, ObjectExtension);
        }
        else
        {
            object = NodejsComponent::getInstance()->getObjects()->get(v8id.value());

            if (object) ENSURE_HAS_EXTENSION(object, nullptr, ObjectExtension);
        }

        if (!object)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<ObjectExtension>(object)));
    }, v8::External::New(isolate, resource))
                                                                   .ToLocalChecked());
}

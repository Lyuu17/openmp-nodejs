
#include <sdk.hpp>

#include "classes/CheckpointExtension.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_HAS_EXTENSION(ext, iext)                                                                 \
    if (!queryExtension<iext>(ext))                                                                     \
    {                                                                                                   \
        menu->addExtension(new iext(menu, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<iext>(menu));                                                             \
    }

CheckpointExtension::CheckpointExtension(IPlayer* player, ResourceManager* resourceManager)
    : m_player(player)
    , m_resourceManager(resourceManager)
{
}

CheckpointExtension::~CheckpointExtension()
{
}

void CheckpointExtension::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void CheckpointExtension::reset()
{
}

// ====================== methods ======================

void CheckpointExtension::isPlayerInside(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), checkpointPlayerData->getCheckpoint().isPlayerInside()));
}

// ====================== accessors ======================

void CheckpointExtension::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(Utils::v8Vector3(checkpointPlayerData->getCheckpoint().getPosition()));
}

void CheckpointExtension::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        return;
    }

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    checkpointPlayerData->getCheckpoint().setPosition(v8vec3.value());
}

void CheckpointExtension::getRadius(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), checkpointPlayerData->getCheckpoint().getRadius()));
}

void CheckpointExtension::setRadius(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        return;
    }

    auto v8rad = Utils::GetDoubleFromV8Value(value);
    if (!v8rad.has_value())
        return;

    checkpointPlayerData->getCheckpoint().setRadius(v8rad.value());
}

void CheckpointExtension::getEnabled(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), checkpointPlayerData->getCheckpoint().isEnabled()));
}

void CheckpointExtension::setEnabled(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto checkpointComponent = (CheckpointExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<CheckpointExtension>(info.GetIsolate(), checkpointComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(checkpointComponent->m_player);
    if (!checkpointPlayerData)
    {
        return;
    }

    auto v8bool = Utils::GetBooleanFromV8Value(value);
    if (!v8bool.has_value())
        return;

    if (v8bool.value())
        checkpointPlayerData->getCheckpoint().enable();
    else
        checkpointPlayerData->getCheckpoint().disable();
}

v8::Local<v8::Object> CheckpointExtension::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("radius", getRadius, setRadius);
    SET_ACCESSOR_WITH_SETTER("enabled", getEnabled, setEnabled);

    return v8obj;
}

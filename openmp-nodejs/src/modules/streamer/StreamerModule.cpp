
#include <sdk.hpp>

#include "modules/streamer/StreamerModule.hpp"
#include "modules/streamer/StreamerObjectExtension.hpp"

#include "V8Class.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

// ====================== methods ======================

void StreamerModule::getDynamicObject(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto resource = (Resource*)info.Data().As<v8::External>()->Value();

    auto streamerComponent = NodejsComponent::getInstance()->getStreamer();
    if (!streamerComponent) return;

    auto id = Utils::GetIntegerFromV8Value(info[0]);

    if (!id.has_value()) return;

    auto obj = streamerComponent->getDynamicObject(id.value());
    if (!obj)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    obj->addExtension(new StreamerObjectExtension(obj, NodejsComponent::getInstance()->getResourceManager()), true);

    auto objExt = queryExtension<StreamerObjectExtension>(obj.get());
    if (!objExt)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    info.GetReturnValue().Set(resource->ObjectFromExtension(objExt));
}

void StreamerModule::createDynamicObject(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto resource = (Resource*)info.Data().As<v8::External>()->Value();

    auto streamerComponent = NodejsComponent::getInstance()->getStreamer();
    if (!streamerComponent) return;

    auto model          = Utils::GetIntegerFromV8Value(info[0]);
    auto pos            = Utils::vector3V8(info[1]);
    auto rot            = Utils::vector3V8(info[2]);
    auto worldId        = Utils::GetIntegerFromV8Value(info[3]);
    auto interiorId     = Utils::GetIntegerFromV8Value(info[4]);
    auto playerId       = Utils::GetIdFromV8Object(info[5]);
    auto streamDistance = Utils::GetDoubleFromV8Value(info[6]);
    auto drawDistance   = Utils::GetDoubleFromV8Value(info[7]);
    auto areaId         = Utils::GetIntegerFromV8Value(info[8]);
    auto priority       = Utils::GetIntegerFromV8Value(info[9]);

    if (!model.has_value() || !pos.has_value() || !rot.has_value()) return;

    auto obj = streamerComponent->createDynamicObject(model.value(), pos.value(), rot.value(), worldId.value_or(-1), interiorId.value_or(-1), playerId.value_or(-1), streamDistance.value_or(STREAMER_OBJECT_SD), drawDistance.value_or(STREAMER_OBJECT_DD), areaId.value_or(-1), priority.value_or(0));
    if (!obj)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    obj->addExtension(new StreamerObjectExtension(obj, NodejsComponent::getInstance()->getResourceManager()), true);

    auto objExt = queryExtension<StreamerObjectExtension>(obj.get());
    if (!objExt)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    info.GetReturnValue().Set(resource->ObjectFromExtension(objExt));
}

void StreamerModule::update(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto resource = (Resource*)info.Data().As<v8::External>()->Value();

    auto streamerComponent = NodejsComponent::getInstance()->getStreamer();
    if (!streamerComponent) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    auto type     = Utils::GetIntegerFromV8Value(info[1]);

    if (!playerId.has_value() || !type.has_value()) return;

    info.GetReturnValue().Set(streamerComponent->update(playerId.value(), (StreamerItemType)type.value()));
}

void StreamerModule::updateEx(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto resource = (Resource*)info.Data().As<v8::External>()->Value();

    auto streamerComponent = NodejsComponent::getInstance()->getStreamer();
    if (!streamerComponent) return;

    auto playerId        = Utils::GetIdFromV8Object(info[0]);
    auto type            = Utils::GetIntegerFromV8Value(info[1]);
    auto pos             = Utils::vector3V8(info[2]);
    auto worldId         = Utils::GetIntegerFromV8Value(info[3]);
    auto interiorId      = Utils::GetIntegerFromV8Value(info[4]);
    auto compensatedTime = Utils::GetIntegerFromV8Value(info[5]);
    auto freezePlayer    = Utils::GetBooleanFromV8Value(info[6]);

    if (!playerId.has_value() || !pos.has_value() || !type.has_value()) return;

    info.GetReturnValue().Set(streamerComponent->updateEx(playerId.value(), pos.value(), worldId.value_or(-1), interiorId.value_or(-1), (StreamerItemType)type.value(), compensatedTime.value_or(-1), freezePlayer.value_or(false)));
}

v8::Local<v8::Object> StreamerModule::CreateJavaScriptObject(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8class = V8Class<StreamerModule>::NewClass("Streamer Module");

#define SET_FUNCTION(f, func) v8class->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, resource)).ToLocalChecked());

    SET_FUNCTION("getDynamicObject", getDynamicObject);
    SET_FUNCTION("createDynamicObject", createDynamicObject);
    SET_FUNCTION("update", update);
    SET_FUNCTION("updateEx", updateEx);

    return v8class.get();
}

void StreamerModule::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = CreateJavaScriptObject(resource);

    context->Global()->Set(context, Utils::v8Str("streamer"), v8obj);
}

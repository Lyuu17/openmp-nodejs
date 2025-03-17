
#include <sdk.hpp>

#include "modules/streamer/StreamerModule.hpp"
#include "modules/streamer/StreamerObjectExtension.hpp"

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

v8::Local<v8::Object> StreamerModule::CreateJavaScriptObject(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, resource)).ToLocalChecked());

    SET_FUNCTION("getDynamicObject", getDynamicObject);
    SET_FUNCTION("createDynamicObject", createDynamicObject);

    return v8obj;
}

void StreamerModule::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = CreateJavaScriptObject(resource);

    context->Global()->Set(context, Utils::v8Str("streamer"), v8obj);
}

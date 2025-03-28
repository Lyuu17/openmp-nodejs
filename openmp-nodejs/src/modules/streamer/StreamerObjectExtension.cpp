
#include <sdk.hpp>

#include "modules/streamer/StreamerObjectExtension.hpp"

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"
#include "V8Class.hpp"

#define ENSURE_OBJECT_HAS_EXTENSION(obj, ext)                                                        \
    if (!queryExtension<ext>(obj))                                                                   \
    {                                                                                                \
        obj->addExtension(new ext(obj, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<ext>(obj));                                                            \
    }

StreamerObjectExtension::StreamerObjectExtension(const std::shared_ptr<streamer::IObject>& obj, ResourceManager* resourceManager)
    : m_object(obj)
    , m_resourceManager(resourceManager)
{
}

StreamerObjectExtension::~StreamerObjectExtension()
{
}

void StreamerObjectExtension::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void StreamerObjectExtension::reset()
{
}

// ====================== methods ======================

void StreamerObjectExtension::destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto streamerExtension = (StreamerObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<StreamerObjectExtension>(info.GetIsolate(), streamerExtension)) return;

    NodejsComponent::getInstance()->getStreamer()->destroyDynamicObject(streamerExtension->m_object->getID());
}

// ====================== accessors ======================

void StreamerObjectExtension::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto streamerExtension = (StreamerObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<StreamerObjectExtension>(info.GetIsolate(), streamerExtension)) return;

    info.GetReturnValue().Set(streamerExtension->m_object->getID());
}

void StreamerObjectExtension::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto streamerExtension = (StreamerObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<StreamerObjectExtension>(info.GetIsolate(), streamerExtension)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(streamerExtension->m_object->getPosition()));
}

void StreamerObjectExtension::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto streamerExtension = (StreamerObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<StreamerObjectExtension>(info.GetIsolate(), streamerExtension)) return;

    auto pos = Utils::vector3V8(value);
    if (!pos.has_value())
        return;

    streamerExtension->m_object->setPosition(pos.value());
}

void StreamerObjectExtension::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto streamerExtension = (StreamerObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<StreamerObjectExtension>(info.GetIsolate(), streamerExtension)) return;

    info.GetReturnValue().Set(Utils::v8Quat(streamerExtension->m_object->getRotation()));
}

void StreamerObjectExtension::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto streamerExtension = (StreamerObjectExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<StreamerObjectExtension>(info.GetIsolate(), streamerExtension)) return;

    auto rot = Utils::quatV8(value);
    if (!rot.has_value())
        return;

    streamerExtension->m_object->setRotation(rot.value());
}

v8::Local<v8::Object> StreamerObjectExtension::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8class = V8Class<StreamerObjectExtension>::NewClass("Dynamic Object");

#define SET_FUNCTION(f, func) v8class->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("destroy", destroy);

#define SET_ACCESSOR(f, getter) v8class->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8class->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("rotation", getRotation, setRotation);

    return v8class.get();
}

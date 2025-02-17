
#pragma once

#include "common.hpp"

#include "NodejsComponent.hpp"

#include <sdk.hpp>

namespace Utils
{
    v8::Local<v8::String>      v8Str(const std::string& x);
    std::optional<std::string> strV8(v8::MaybeLocal<v8::Value>);

    v8::Local<v8::Object>  v8Vector2(Vector2 vec2);
    v8::Local<v8::Object>  v8Vector3(Vector3 vec3);
    std::optional<Vector3> vector3V8(v8::MaybeLocal<v8::Value> val);
    v8::Local<v8::Object>  v8Colour(const Colour& col);
    std::optional<Colour>  colourV8(v8::MaybeLocal<v8::Value> val);

    std::optional<GTAQuat> quatV8(v8::MaybeLocal<v8::Value> val);
    v8::Local<v8::Object>  v8Quat(GTAQuat quat);

    v8::Local<v8::Object> v8PlayerBulletData(const PlayerBulletData& playerBulletData);

    v8::Local<v8::Object> CancellableEventObject();

    std::optional<int32_t>  GetIntegerFromV8Value(v8::MaybeLocal<v8::Value> val);
    std::optional<double>   GetDoubleFromV8Value(v8::MaybeLocal<v8::Value> val);
    std::optional<bool>     GetBooleanFromV8Value(v8::MaybeLocal<v8::Value> val);
    std::optional<uint32_t> GetIdFromV8Object(v8::MaybeLocal<v8::Value> val);

    std::optional<WeaponSlots> GetWeaponSlotsDataFromV8Object(v8::MaybeLocal<v8::Value> val);

    void PrintWavyUnderline(int start, int length);
    int  CountLeadingSpaces(const char* str, int length);
    void PrintSourceLineWithUnderline(v8::Isolate* isolate, v8::Local<v8::Message> message, v8::Local<v8::Context> context);

    template <typename T>
    constexpr auto CheckExtensionExist = [](v8::Isolate* isolate, T* component) {
        auto resource = NodejsComponent::getInstance()->getResourceManager()->GetResourceFromIsolate(isolate);
        if (!resource->DoesObjectFromExtensionExist(component))
        {
            resource->ThrowException("attempting to access a deleted component");
            return false;
        }
        return true;
    };
} // namespace Utils

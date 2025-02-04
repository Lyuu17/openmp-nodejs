
#pragma once

#include "common.hpp"

#include <sdk.hpp>

namespace Utils
{
    v8::Local<v8::String> v8Str(const std::string& x);
    std::string           strV8(v8::Local<v8::String>);

    v8::Local<v8::Object> v8Vector3(Vector3 vec3);
    Vector3               vector3V8(v8::Local<v8::Object> vec3Obj);

    v8::Local<v8::Object> CancellableEventObject();

    std::optional<int32_t>  GetIntegerFromV8Value(v8::MaybeLocal<v8::Value> val);
    std::optional<double>   GetDoubleFromV8Value(v8::MaybeLocal<v8::Value> val);
    std::optional<uint32_t> GetPlayerIdFromV8Object(v8::MaybeLocal<v8::Object> val);

    void PrintWavyUnderline(int start, int length);
    int  CountLeadingSpaces(const char* str, int length);
    void PrintSourceLineWithUnderline(v8::Isolate* isolate, v8::Local<v8::Message> message, v8::Local<v8::Context> context);
} // namespace Utils

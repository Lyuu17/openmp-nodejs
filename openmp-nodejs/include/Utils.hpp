
#pragma once

#include "common.hpp"

#include <sdk.hpp>

namespace Utils
{
    v8::Local<v8::String> v8Str(const std::string& x);
    std::string           strV8(v8::Local<v8::String>);

    v8::Local<v8::Object> v8Vector3(Vector3 vec3);
    Vector3               vector3V8(v8::Local<v8::Object> vec3Obj);

    void PrintWavyUnderline(int start, int length);
    int  CountLeadingSpaces(const char* str, int length);
    void PrintSourceLineWithUnderline(v8::Isolate* isolate, v8::Local<v8::Message> message, v8::Local<v8::Context> context);
} // namespace Utils

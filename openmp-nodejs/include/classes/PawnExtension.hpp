
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <sdk.hpp>

struct PawnExtension final
{
    static void callPublicFunction(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void callNativeFunction(const v8::FunctionCallbackInfo<v8::Value>& info);

    static v8::Local<v8::Object> CreateJavaScriptObject();

    static void InitFunctions(Resource*);
};

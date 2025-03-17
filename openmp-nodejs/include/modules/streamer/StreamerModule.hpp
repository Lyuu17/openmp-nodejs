
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <StreamerComponent.hpp>

#include <sdk.hpp>

struct StreamerModule final
{
    static void getDynamicObject(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void createDynamicObject(const v8::FunctionCallbackInfo<v8::Value>& info);

    static v8::Local<v8::Object> CreateJavaScriptObject(Resource* resource);

    static void InitFunctions(Resource*);
};

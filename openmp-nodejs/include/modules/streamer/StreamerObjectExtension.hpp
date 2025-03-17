
#pragma once

#include "common.hpp"
#include "ResourceManager.hpp"
#include <StreamerComponent.hpp>
#include <sdk.hpp>

struct StreamerObjectExtension final : public IExtension,
                                       public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0x9BAB5386264024ED);

    std::shared_ptr<streamer::IObject> m_object;
    ResourceManager*                   m_resourceManager;

    StreamerObjectExtension(const std::shared_ptr<streamer::IObject>&, ResourceManager*);
    ~StreamerObjectExtension();

    void freeExtension() override;
    void reset() override;

    static void destroy(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;
};

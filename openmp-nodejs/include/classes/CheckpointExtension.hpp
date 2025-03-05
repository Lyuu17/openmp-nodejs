
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Checkpoints/checkpoints.hpp>
#include <sdk.hpp>

struct CheckpointExtension final : public IExtension,
                                   public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0xF4447713D7DF773E);

    IPlayer*         m_player;
    ResourceManager* m_resourceManager;

    CheckpointExtension(IPlayer*, ResourceManager*);
    ~CheckpointExtension();

    void freeExtension() override;
    void reset() override;

    static void isPlayerInside(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getRadius(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setRadius(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getEnabled(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setEnabled(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;
};

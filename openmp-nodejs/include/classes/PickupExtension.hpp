
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Pickups/pickups.hpp>
#include <sdk.hpp>

struct PickupExtension final : public IExtension,
                               public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0x100B80797F3FA5BF);

    IPickup*         m_pickup;
    ResourceManager* m_resourceManager;

    PickupExtension(IPickup*, ResourceManager*);
    ~PickupExtension();

    void freeExtension() override;
    void reset() override;

    static void destroy(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setPickupHiddenForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void isPickupHiddenForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getType(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setType(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setModel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};


#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

struct VehicleComponent final : public IExtension,
                                public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0xC4E68E76688A82EF);

    IVehicle*        m_vehicle;
    ResourceManager* m_resourceManager;

    VehicleComponent(IVehicle*, ResourceManager*);
    ~VehicleComponent();

    void freeExtension() override;
    void reset() override;

    static void repair(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void putPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getDriver(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPlate(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPlate(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};


#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

enum class VehicleParamType
{
    DOORS,
    BONNET,
    BOOT,
    OBJECTIVE,
    SIREN,
    DOOR_DRIVER,
    DOOR_PASSENGER,
    DOOR_BACKLEFT,
    DOOR_BACKRIGHT,
    WINDOW_DRIVER,
    WINDOW_PASSENGER,
    WINDOW_BACKLEFT,
    WINDOW_BACKRIGHT,
};

struct VehicleExtension final : public IExtension,
                                public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0xC4E68E76688A82EF);

    IVehicle*        m_vehicle;
    ResourceManager* m_resourceManager;

    VehicleExtension(IVehicle*, ResourceManager*);
    ~VehicleExtension();

    void freeExtension() override;
    void reset() override;

    static void repair(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void putPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void addComponent(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getComponentInSlot(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void removeComponent(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getParam(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setParam(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getDriver(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPassengers(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPlate(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPlate(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getEngine(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setEngine(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getLights(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setLights(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getAlarm(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setAlarm(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};


#pragma once

#include "common.hpp"

#include "Resource.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

struct PlayerComponent final : public IExtension,
                               public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0x88FB0AC44120E3E8);

    IPlayer* m_player;

    PlayerComponent(IPlayer*);
    ~PlayerComponent();

    void freeExtension() override;
    void reset() override;

    static void kick(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void ban(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void giveMoney(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void giveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void removeWeapon(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setWeaponAmmo(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getWeapons(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getWeaponSlot(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void resetWeapons(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getMoney(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setMoney(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getArmour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setArmour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getWeapon(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setWeapon(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getWeaponAmmo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    v8::Local<v8::Object> CreateJavaScriptObject(Resource* resource) override;
};

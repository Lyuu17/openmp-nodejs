
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

struct PlayerComponent final : public IExtension,
                               public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0x88FB0AC44120E3E8);

    IPlayer*         m_player;
    ResourceManager* m_resourceManager;

    PlayerComponent(IPlayer*, ResourceManager*);
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
    static void spawn(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void forceClassSelection(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getMoney(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setMoney(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getScore(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setScore(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getSkin(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setSkin(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getInterior(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setInterior(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getTeam(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setTeam(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getArmour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setArmour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getWeapon(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setWeapon(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getWeaponAmmo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getDrunkLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setDrunkLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getWantedLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setWantedLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getControllable(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setControllable(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getSpawnInfo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setSpawnInfo(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};

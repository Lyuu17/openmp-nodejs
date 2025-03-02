
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

    v8::Global<v8::Function> m_dialogCallback;

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
    static void hideDialog(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void showDialog(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setMapIcon(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void unsetMapIcon(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void sendGameText(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getCameraPosition(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getCameraLookAt(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setCameraPosition(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setCameraLookAt(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setCameraBehind(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void beginSelection(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void endSelection(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void beginObjectSelection(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void endObjectSelection(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void beginObjectEdit(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void endObjectEdit(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void attachCameraToObject(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

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

    static void getVehicle(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getIp(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getGhostMode(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setGhostMode(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getSpecialAction(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setSpecialAction(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getMenu(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setMenu(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getDialog(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getShopName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setShopName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getCheckpoint(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void isSelecting(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getKeys(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getSelectingObject(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getEditingObject(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getSpectating(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setSpectating(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};

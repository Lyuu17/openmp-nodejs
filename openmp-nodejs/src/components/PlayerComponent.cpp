
#include <Server/Components/Classes/classes.hpp>
#include <sdk.hpp>

#include "components/PlayerComponent.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

#define CHECK_EXTENSION_EXIST(isolate, component)                             \
    auto resource = ResourceManager::GetResourceFromIsolate(isolate);         \
    if (!resource->DoesObjectFromExtensionExist(component))                   \
    {                                                                         \
        resource->ThrowException("attempting to access a deleted component"); \
        return;                                                               \
    }

PlayerComponent::PlayerComponent(IPlayer* player)
    : m_player(player)
{
}

PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::freeExtension()
{
    ResourceManager::Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void PlayerComponent::reset()
{
}

// ====================== methods ======================

void PlayerComponent::kick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    playerComponent->m_player->kick();

    info.GetReturnValue().Set(true);
}

void PlayerComponent::ban(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    std::string reason {};
    if (info[0]->IsString())
        reason = Utils::strV8(info[0]->ToString(info.GetIsolate()->GetCurrentContext()).ToLocalChecked());

    playerComponent->m_player->ban(reason.c_str());

    info.GetReturnValue().Set(true);
}

void PlayerComponent::giveMoney(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->giveMoney(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerComponent::giveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8intWeapon = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intWeapon.IsEmpty())
        return;

    auto v8intAmmo = info[1]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intAmmo.IsEmpty())
        return;

    WeaponSlotData weaponSlotData {
        (uint8_t)v8intWeapon.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked(),
        (uint32_t)v8intAmmo.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked()
    };

    playerComponent->m_player->giveWeapon(weaponSlotData);

    info.GetReturnValue().Set(true);
}

void PlayerComponent::removeWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8intWeapon = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intWeapon.IsEmpty())
        return;

    playerComponent->m_player->removeWeapon(v8intWeapon.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(true);
}

void PlayerComponent::setWeaponAmmo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8intWeapon = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intWeapon.IsEmpty())
        return;

    auto v8intAmmo = info[1]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intAmmo.IsEmpty())
        return;

    WeaponSlotData weaponSlotData {
        (uint8_t)v8intWeapon.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked(),
        (uint32_t)v8intAmmo.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked()
    };

    playerComponent->m_player->setWeaponAmmo(weaponSlotData);

    info.GetReturnValue().Set(true);
}

void PlayerComponent::getWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto& weaponSlots = playerComponent->m_player->getWeapons();

    auto v8obj = v8::Object::New(info.GetIsolate());

    for (const auto& [wep, ammo] : weaponSlots)
        v8obj->Set(info.GetIsolate()->GetCurrentContext(), wep, v8::Number::New(info.GetIsolate(), ammo));

    info.GetReturnValue().Set(v8obj);
}

void PlayerComponent::getWeaponSlot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8intWeapon = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intWeapon.IsEmpty())
        return;

    auto weaponData = playerComponent->m_player->getWeaponSlot(v8intWeapon.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    auto v8obj = v8::Object::New(info.GetIsolate());

    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("id"), v8::Number::New(info.GetIsolate(), weaponData.id));
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("ammo"), v8::Number::New(info.GetIsolate(), weaponData.ammo));

    info.GetReturnValue().Set(v8obj);
}

void PlayerComponent::resetWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    playerComponent->m_player->resetWeapons();

    info.GetReturnValue().Set(true);
}

// ====================== accessors ======================

void PlayerComponent::getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().data()));
}

void PlayerComponent::setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8str = value->ToString(info.GetIsolate()->GetCurrentContext());
    if (v8str.IsEmpty())
        return;

    playerComponent->m_player->setName(Utils::strV8(v8str.ToLocalChecked()));

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().to_string().c_str()));
}

void PlayerComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getID()));
}

void PlayerComponent::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getPosition()));
}

void PlayerComponent::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto mayv8vector3Obj = value->ToObject(info.GetIsolate()->GetCurrentContext());
    if (mayv8vector3Obj.IsEmpty())
        return;

    playerComponent->m_player->setPosition(Utils::vector3V8(mayv8vector3Obj.ToLocalChecked()));

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getPosition()));
}

void PlayerComponent::getMoney(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().data()));
}

void PlayerComponent::setMoney(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setMoney(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerComponent::getScore(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getScore()));
}

void PlayerComponent::setScore(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setScore(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getScore()));
}

void PlayerComponent::getSkin(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getSkin()));
}

void PlayerComponent::setSkin(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setSkin(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getSkin()));
}

void PlayerComponent::getInterior(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getInterior()));
}

void PlayerComponent::setInterior(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setInterior(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getInterior()));
}

void PlayerComponent::getTeam(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getTeam()));
}

void PlayerComponent::setTeam(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setInterior(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getTeam()));
}

void PlayerComponent::getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getHealth()));
}

void PlayerComponent::setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setHealth(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getHealth()));
}

void PlayerComponent::getArmour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmour()));
}

void PlayerComponent::setArmour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setArmour(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmour()));
}

void PlayerComponent::getWeapon(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeapon()));
}

void PlayerComponent::setWeapon(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setArmedWeapon(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeapon()));
}

void PlayerComponent::getWeaponAmmo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeaponAmmo()));
}

void PlayerComponent::getDrunkLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getDrunkLevel()));
}

void PlayerComponent::setDrunkLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setDrunkLevel(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getDrunkLevel()));
}

void PlayerComponent::getWantedLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getWantedLevel()));
}

void PlayerComponent::setWantedLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    CHECK_EXTENSION_EXIST(info.GetIsolate(), playerComponent);

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setWantedLevel(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getWantedLevel()));
}

v8::Local<v8::Object> PlayerComponent::CreateJavaScriptObject(Resource* resource)
{
    auto isolate = resource->m_isolate;
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("kick", kick)
    SET_FUNCTION("ban", ban)
    SET_FUNCTION("giveMoney", giveMoney)
    SET_FUNCTION("giveWeapon", giveWeapon)
    SET_FUNCTION("removeWeapon", removeWeapon)
    SET_FUNCTION("setWeaponAmmo", setWeaponAmmo)
    SET_FUNCTION("getWeapons", getWeapons)

#define SET_ACCESSOR(f, getter) v8obj->SetAccessor(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetAccessor(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR_WITH_SETTER("name", getName, setName);
    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("money", getMoney, setMoney);
    SET_ACCESSOR_WITH_SETTER("score", getScore, setScore);
    SET_ACCESSOR_WITH_SETTER("skin", getSkin, setSkin);
    SET_ACCESSOR_WITH_SETTER("interior", getInterior, setInterior);
    SET_ACCESSOR_WITH_SETTER("team", getTeam, setTeam);
    SET_ACCESSOR_WITH_SETTER("health", getHealth, setHealth);
    SET_ACCESSOR_WITH_SETTER("armour", getArmour, setArmour);
    SET_ACCESSOR_WITH_SETTER("weapon", getWeapon, setWeapon);
    SET_ACCESSOR("weaponAmmo", getWeaponAmmo);
    SET_ACCESSOR_WITH_SETTER("drunkLevel", getDrunkLevel, setDrunkLevel);
    SET_ACCESSOR_WITH_SETTER("wantedLevel", getWantedLevel, setWantedLevel);

    return v8obj;
}


#include <sdk.hpp>

#include "components/PlayerComponent.hpp"
#include "components/VehicleComponent.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

PlayerComponent::PlayerComponent(IPlayer* player, ResourceManager* resourceManager)
    : m_player(player)
    , m_resourceManager(resourceManager)
{
}

PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
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

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->kick();
}

void PlayerComponent::ban(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    std::string reason {};
    if (info[0]->IsString())
        reason = Utils::strV8(info[0]->ToString(info.GetIsolate()->GetCurrentContext()).ToLocalChecked());

    playerComponent->m_player->ban(reason.c_str());
}

void PlayerComponent::giveMoney(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->giveMoney(v8int.value());

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerComponent::giveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8intWeapon = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8intWeapon.has_value())
        return;

    auto v8intAmmo = Utils::GetIntegerFromV8Value(info[1]);
    if (!v8intAmmo.has_value())
        return;

    WeaponSlotData weaponSlotData {
        (uint8_t)v8intWeapon.value(),
        (uint32_t)v8intAmmo.value()
    };

    playerComponent->m_player->giveWeapon(weaponSlotData);
}

void PlayerComponent::removeWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8intWeapon = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8intWeapon.has_value())
        return;

    playerComponent->m_player->removeWeapon(v8intWeapon.value());
}

void PlayerComponent::setWeaponAmmo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8intWeapon = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8intWeapon.has_value())
        return;

    auto v8intAmmo = Utils::GetIntegerFromV8Value(info[1]);
    if (!v8intAmmo.has_value())
        return;

    WeaponSlotData weaponSlotData {
        (uint8_t)v8intWeapon.value(),
        (uint32_t)v8intAmmo.value()
    };

    playerComponent->m_player->setWeaponAmmo(weaponSlotData);
}

void PlayerComponent::getWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto& weaponSlots = playerComponent->m_player->getWeapons();

    auto v8obj = v8::Array::New(info.GetIsolate());

    uint32_t wepIdInArray = 0;
    for (const auto& [wep, ammo] : weaponSlots)
    {
        if (ammo == 0)
            continue;

        auto v8wep = v8::Object::New(info.GetIsolate());
        v8wep->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("id"), v8::Number::New(info.GetIsolate(), wep));
        v8wep->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("ammo"), v8::Number::New(info.GetIsolate(), ammo));

        v8obj->Set(info.GetIsolate()->GetCurrentContext(), wepIdInArray, v8wep);

        wepIdInArray++;
    }

    info.GetReturnValue().Set(v8obj);
}

void PlayerComponent::getWeaponSlot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8intWeapon = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8intWeapon.has_value())
        return;

    auto weaponData = playerComponent->m_player->getWeaponSlot(v8intWeapon.value());

    auto v8obj = v8::Object::New(info.GetIsolate());

    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("id"), v8::Number::New(info.GetIsolate(), weaponData.id));
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("ammo"), v8::Number::New(info.GetIsolate(), weaponData.ammo));

    info.GetReturnValue().Set(v8obj);
}

void PlayerComponent::resetWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->resetWeapons();
}

void PlayerComponent::spawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerClassData = queryExtension<IPlayerClassData>(playerComponent->m_player);
    if (!playerClassData)
    {
        playerComponent->m_player->spawn();
    }
    else
    {
        playerClassData->spawnPlayer();
    }

    NodejsComponent::getInstance()->getResourceManager()->Exec([&playerComponent](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(playerComponent->m_player));
        resource->Emit("onPlayerSpawn", { v8objPlayer }); // onPlayerSpawn isn't called when player is forced to spawn
    });
}

void PlayerComponent::forceClassSelection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->forceClassSelection();
}

void PlayerComponent::hideDialog(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerDialogData = queryExtension<IPlayerDialogData>(playerComponent->m_player);
    if (!playerDialogData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    playerDialogData->hide(*playerComponent->m_player);
}

void PlayerComponent::showDialog(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerDialogData = queryExtension<IPlayerDialogData>(playerComponent->m_player);
    if (!playerDialogData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    auto v8dialogId    = Utils::GetIntegerFromV8Value(info[0]);
    auto v8dialogStyle = Utils::GetIntegerFromV8Value(info[1]);
    auto v8title       = info[2]->ToString(info.GetIsolate()->GetCurrentContext());
    auto v8body        = info[3]->ToString(info.GetIsolate()->GetCurrentContext());
    auto v8button1     = info[4]->ToString(info.GetIsolate()->GetCurrentContext());
    auto v8button2     = info[5]->ToString(info.GetIsolate()->GetCurrentContext());

    if (!v8dialogId.has_value() || !v8dialogStyle.has_value() || v8title.IsEmpty() || v8body.IsEmpty() || v8button1.IsEmpty() || v8button2.IsEmpty())
        return;

    auto title   = Utils::strV8(v8title.ToLocalChecked());
    auto body    = Utils::strV8(v8body.ToLocalChecked());
    auto button1 = Utils::strV8(v8button1.ToLocalChecked());
    auto button2 = Utils::strV8(v8button2.ToLocalChecked());

    playerDialogData->show(*playerComponent->m_player, v8dialogId.value(), (DialogStyle)v8dialogStyle.value(), title, body, button1, button2);
}

// ====================== accessors ======================

void PlayerComponent::getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().data()));
}

void PlayerComponent::setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8str = value->ToString(info.GetIsolate()->GetCurrentContext());
    if (v8str.IsEmpty())
        return;

    playerComponent->m_player->setName(Utils::strV8(v8str.ToLocalChecked()));
}

void PlayerComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getID()));
}

void PlayerComponent::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getPosition()));
}

void PlayerComponent::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    playerComponent->m_player->setPosition(v8vec3.value());
}

void PlayerComponent::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Quat(playerComponent->m_player->getRotation()));
}

void PlayerComponent::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8quat = Utils::quatV8(value);
    if (!v8quat.has_value())
        return;

    playerComponent->m_player->setRotation(v8quat.value());
}

void PlayerComponent::getMoney(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerComponent::setMoney(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setMoney(v8int.value());
}

void PlayerComponent::getScore(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getScore()));
}

void PlayerComponent::setScore(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setScore(v8int.value());
}

void PlayerComponent::getSkin(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getSkin()));
}

void PlayerComponent::setSkin(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setSkin(v8int.value());
}

void PlayerComponent::getInterior(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getInterior()));
}

void PlayerComponent::setInterior(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setInterior(v8int.value());
}

void PlayerComponent::getTeam(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getTeam()));
}

void PlayerComponent::setTeam(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setInterior(v8int.value());
}

void PlayerComponent::getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getHealth()));
}

void PlayerComponent::setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setHealth(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

void PlayerComponent::getArmour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmour()));
}

void PlayerComponent::setArmour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setArmour(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

void PlayerComponent::getWeapon(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeapon()));
}

void PlayerComponent::setWeapon(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setArmedWeapon(v8int.value());
}

void PlayerComponent::getWeaponAmmo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeaponAmmo()));
}

void PlayerComponent::getDrunkLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getDrunkLevel()));
}

void PlayerComponent::setDrunkLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setDrunkLevel(v8int.value());
}

void PlayerComponent::getWantedLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getWantedLevel()));
}

void PlayerComponent::setWantedLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setWantedLevel(v8int.value());
}

void PlayerComponent::getControllable(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getControllable()));
}

void PlayerComponent::setControllable(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8bool = value->ToBoolean(info.GetIsolate());
    if (v8bool.IsEmpty())
        return;

    playerComponent->m_player->setControllable(v8bool->Value());
}

void PlayerComponent::getSpawnInfo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerClassData = queryExtension<IPlayerClassData>(playerComponent->m_player);
    if (!playerClassData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    auto context = info.GetIsolate()->GetCurrentContext();

    auto& playerClass = playerClassData->getClass();

    auto v8playerClass = v8::Object::New(info.GetIsolate());
    v8playerClass->Set(context, Utils::v8Str("team"), v8::Integer::New(info.GetIsolate(), playerClass.team));
    v8playerClass->Set(context, Utils::v8Str("skin"), v8::Integer::New(info.GetIsolate(), playerClass.skin));
    v8playerClass->Set(context, Utils::v8Str("spawn"), Utils::v8Vector3(playerClass.spawn));
    v8playerClass->Set(context, Utils::v8Str("angle"), v8::Number::New(info.GetIsolate(), playerClass.angle));

    auto v8weaponSlotList = v8::Array::New(info.GetIsolate());

    uint32_t wepIdInArray = 0;
    for (auto& weaponSlotData : playerClass.weapons)
    {
        if (weaponSlotData.ammo == 0)
            continue;

        auto v8weaponSlot = v8::Object::New(info.GetIsolate());
        v8weaponSlot->Set(context, Utils::v8Str("id"), v8::Integer::New(info.GetIsolate(), weaponSlotData.id));
        v8weaponSlot->Set(context, Utils::v8Str("ammo"), v8::Integer::New(info.GetIsolate(), weaponSlotData.ammo));

        v8weaponSlotList->Set(context, wepIdInArray, v8weaponSlot);

        wepIdInArray++;
    }

    v8playerClass->Set(context, Utils::v8Str("weapons"), v8weaponSlotList);

    info.GetReturnValue().Set(v8playerClass);
}

void PlayerComponent::setSpawnInfo(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerClassData = queryExtension<IPlayerClassData>(playerComponent->m_player);
    if (!playerClassData)
    {
        return;
    }

    if (!value->IsObject())
        return;

    auto context = info.GetIsolate()->GetCurrentContext();

    auto v8obj = value.As<v8::Object>();

    auto v8team        = Utils::GetIntegerFromV8Value(v8obj->Get(context, Utils::v8Str("team")));
    auto v8skin        = Utils::GetIntegerFromV8Value(v8obj->Get(context, Utils::v8Str("skin")));
    auto v8spawn       = Utils::vector3V8(v8obj->Get(context, Utils::v8Str("spawn")));
    auto v8angle       = Utils::GetDoubleFromV8Value(v8obj->Get(context, Utils::v8Str("angle")));
    auto v8weaponSlots = Utils::GetWeaponSlotsDataFromV8Object(v8obj->Get(context, Utils::v8Str("weapons")));

    PlayerClass playerClass {
        v8skin.value_or(0),
        v8team.value_or(0),
        v8spawn.value_or(Vector3 {}),
        (float)v8angle.value_or(0),
        v8weaponSlots.value_or(WeaponSlots {})
    };

    playerClassData->setSpawnInfo(playerClass);
}

void PlayerComponent::getVehicle(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerVehicleData = queryExtension<IPlayerVehicleData>(playerComponent->m_player);
    if (!playerVehicleData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    if (!playerVehicleData->getVehicle())
    {
        info.GetReturnValue().SetNull();
        return;
    }

    auto context = info.GetIsolate()->GetCurrentContext();

    auto vehicleComponent = queryExtension<VehicleComponent>(playerVehicleData->getVehicle());
    if (!vehicleComponent)
    {
        playerVehicleData->getVehicle()->addExtension(new VehicleComponent(playerVehicleData->getVehicle(), NodejsComponent::getInstance()->getResourceManager()), true);

        vehicleComponent = queryExtension<VehicleComponent>(playerVehicleData->getVehicle());

        assert(vehicleComponent);
    }

    auto resource = NodejsComponent::getInstance()->getResourceManager()->GetResourceFromIsolate(info.GetIsolate());
    info.GetReturnValue().Set(resource->ObjectFromExtension(vehicleComponent));
}

void PlayerComponent::getIp(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    PeerAddress::AddressString addressStr;

    PeerAddress::ToString(playerComponent->m_player->getNetworkData().networkID.address, addressStr);

    info.GetReturnValue().Set(Utils::v8Str(addressStr.data()));
}

void PlayerComponent::getGhostMode(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(playerComponent->m_player->isGhostModeEnabled());
}

void PlayerComponent::setGhostMode(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8bool = Utils::GetBooleanFromV8Value(value);
    if (!v8bool.has_value())
        return;

    playerComponent->m_player->toggleGhostMode(v8bool.value());
}

void PlayerComponent::getSpecialAction(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(playerComponent->m_player->getAction());
}

void PlayerComponent::setSpecialAction(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto v8action = Utils::GetIntegerFromV8Value(value);
    if (!v8action.has_value())
        return;

    playerComponent->m_player->setAction((PlayerSpecialAction)v8action.value());
}

void PlayerComponent::getMenu(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerMenuData = queryExtension<IPlayerMenuData>(playerComponent->m_player);
    if (!playerMenuData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    info.GetReturnValue().Set(playerMenuData->getMenuID());
}

void PlayerComponent::setMenu(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerMenuData = queryExtension<IPlayerMenuData>(playerComponent->m_player);
    if (!playerMenuData)
        return;

    auto v8menu = Utils::GetIntegerFromV8Value(value);
    if (!v8menu.has_value())
        return;

    playerMenuData->setMenuID(v8menu.value());
}

void PlayerComponent::getDialog(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerComponent>(info.GetIsolate(), playerComponent)) return;

    auto playerDialogData = queryExtension<IPlayerDialogData>(playerComponent->m_player);
    if (!playerDialogData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    info.GetReturnValue().Set(playerDialogData->getActiveID());
}

v8::Local<v8::Object> PlayerComponent::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("kick", kick);
    SET_FUNCTION("ban", ban);
    SET_FUNCTION("giveMoney", giveMoney);
    SET_FUNCTION("giveWeapon", giveWeapon);
    SET_FUNCTION("removeWeapon", removeWeapon);
    SET_FUNCTION("setWeaponAmmo", setWeaponAmmo);
    SET_FUNCTION("getWeapons", getWeapons);
    SET_FUNCTION("getWeaponSlot", getWeaponSlot);
    SET_FUNCTION("resetWeapons", resetWeapons);
    SET_FUNCTION("spawn", spawn);
    SET_FUNCTION("forceClassSelection", forceClassSelection);
    SET_FUNCTION("hideDialog", hideDialog);
    SET_FUNCTION("showDialog", showDialog);

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR_WITH_SETTER("name", getName, setName);
    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("rotation", getRotation, setRotation);
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
    SET_ACCESSOR_WITH_SETTER("controllable", getControllable, setControllable);
    SET_ACCESSOR_WITH_SETTER("spawnInfo", getSpawnInfo, setSpawnInfo);
    SET_ACCESSOR("vehicle", getVehicle);
    SET_ACCESSOR("ip", getIp);
    SET_ACCESSOR_WITH_SETTER("ghostMode", getGhostMode, setGhostMode);
    SET_ACCESSOR_WITH_SETTER("specialAction", getSpecialAction, setSpecialAction);
    SET_ACCESSOR_WITH_SETTER("menu", getMenu, setMenu);
    SET_ACCESSOR("dialog", getDialog);

    return v8obj;
}

void PlayerComponent::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("getPlayer"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8int = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
        if (v8int.IsEmpty())
            return;

        auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(v8int.ToLocalChecked()->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked());
        if (!player)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<PlayerComponent>(player)));
    }, v8::External::New(isolate, resource))
                                                                   .ToLocalChecked());
}

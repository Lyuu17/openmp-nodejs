
#include <sdk.hpp>

#include "classes/PlayerExtension.hpp"
#include "classes/VehicleExtension.hpp"
#include "classes/CheckpointExtension.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

PlayerExtension::PlayerExtension(IPlayer* player, ResourceManager* resourceManager)
    : m_player(player)
    , m_resourceManager(resourceManager)
{
}

PlayerExtension::~PlayerExtension()
{
    m_dialogCallback.Reset();
}

void PlayerExtension::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void PlayerExtension::reset()
{
}

// ====================== methods ======================

void PlayerExtension::kick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->kick();
}

void PlayerExtension::ban(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto reason = Utils::strV8(info[0]);

    playerComponent->m_player->ban(reason.value_or(""));
}

void PlayerExtension::giveMoney(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->giveMoney(v8int.value());

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerExtension::giveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

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

void PlayerExtension::removeWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8intWeapon = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8intWeapon.has_value())
        return;

    playerComponent->m_player->removeWeapon(v8intWeapon.value());
}

void PlayerExtension::setWeaponAmmo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

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

void PlayerExtension::getWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

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

void PlayerExtension::getWeaponSlot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8intWeapon = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8intWeapon.has_value())
        return;

    auto weaponData = playerComponent->m_player->getWeaponSlot(v8intWeapon.value());

    auto v8obj = v8::Object::New(info.GetIsolate());

    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("id"), v8::Number::New(info.GetIsolate(), weaponData.id));
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("ammo"), v8::Number::New(info.GetIsolate(), weaponData.ammo));

    info.GetReturnValue().Set(v8obj);
}

void PlayerExtension::resetWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->resetWeapons();
}

void PlayerExtension::spawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

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
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(playerComponent->m_player));
        resource->Emit("onPlayerSpawn", { v8objPlayer }); // onPlayerSpawn isn't called when player is forced to spawn
    });
}

void PlayerExtension::forceClassSelection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->forceClassSelection();
}

void PlayerExtension::hideDialog(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerDialogData = queryExtension<IPlayerDialogData>(playerComponent->m_player);
    if (!playerDialogData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    playerComponent->m_dialogCallback.Reset();

    playerDialogData->hide(*playerComponent->m_player);
}

void PlayerExtension::showDialog(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerDialogData = queryExtension<IPlayerDialogData>(playerComponent->m_player);
    if (!playerDialogData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    auto v8dialogStyle = Utils::GetIntegerFromV8Value(info[0]);
    auto v8title       = Utils::strV8(info[1]);
    auto v8body        = Utils::strV8(info[2]);
    auto v8button1     = Utils::strV8(info[3]);
    auto v8button2     = Utils::strV8(info[4]);
    auto v8cb          = info[5];

    if (!v8dialogStyle.has_value() || !v8title.has_value() || !v8body.has_value() || !v8button1.has_value() || !v8button2.has_value() || v8cb.IsEmpty() || !v8cb->IsFunction())
        return;

    playerComponent->m_dialogCallback.Reset(info.GetIsolate(), v8cb.As<v8::Function>());

    playerDialogData->show(*playerComponent->m_player, 0, (DialogStyle)v8dialogStyle.value(), v8title.value(), v8body.value(), v8button1.value(), v8button2.value());
}

void PlayerExtension::setMapIcon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8mapIcon      = Utils::GetIntegerFromV8Value(info[0]);
    auto v8position     = Utils::vector3V8(info[1]);
    auto v8type         = Utils::GetIntegerFromV8Value(info[2]);
    auto v8color        = Utils::colourV8(info[3]);
    auto v8mapIconStyle = Utils::GetIntegerFromV8Value(info[4]);

    if (!v8mapIcon.has_value() || !v8position.has_value() || !v8type.has_value() || !v8color.has_value() || !v8mapIconStyle.has_value())
        return;

    playerComponent->m_player->setMapIcon(v8mapIcon.value(), v8position.value(), v8type.value(), v8color.value(), (MapIconStyle)v8mapIconStyle.value());
}

void PlayerExtension::unsetMapIcon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8mapIcon = Utils::GetIntegerFromV8Value(info[0]);

    if (!v8mapIcon.has_value())
        return;

    playerComponent->m_player->unsetMapIcon(v8mapIcon.value());
}

void PlayerExtension::sendGameText(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8message = Utils::strV8(info[0]);
    auto v8time    = Utils::GetIntegerFromV8Value(info[1]);
    auto v8style   = Utils::GetIntegerFromV8Value(info[2]);

    if (!v8message.has_value() || !v8time.has_value() || !v8style.has_value())
        return;

    auto duration = std::chrono::milliseconds(v8time.value());
    playerComponent->m_player->sendGameText(v8message.value(), duration, v8style.value());
}

void PlayerExtension::getCameraPosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getCameraPosition()));
}

void PlayerExtension::getCameraLookAt(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getCameraLookAt()));
}

void PlayerExtension::getCameraFrontVector(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getAimData().camFrontVector));
}

void PlayerExtension::setCameraPosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8pos = Utils::vector3V8(info[0]);
    if (!v8pos.has_value())
        return;

    playerComponent->m_player->setCameraPosition(v8pos.value());
}

void PlayerExtension::setCameraLookAt(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8pos     = Utils::vector3V8(info[0]);
    auto v8cutType = Utils::GetIntegerFromV8Value(info[1]);
    if (!v8pos.has_value() || !v8cutType.has_value())
        return;

    playerComponent->m_player->setCameraLookAt(v8pos.value(), v8cutType.value());
}

void PlayerExtension::setCameraBehind(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    playerComponent->m_player->setCameraBehind();
}

void PlayerExtension::beginSelection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerTextDrawData = queryExtension<IPlayerTextDrawData>(playerComponent->m_player);
    if (!playerTextDrawData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    auto v8col = Utils::colourV8(info[0]);
    if (!v8col.has_value())
        return;

    playerTextDrawData->beginSelection(v8col.value());
}

void PlayerExtension::endSelection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerTextDrawData = queryExtension<IPlayerTextDrawData>(playerComponent->m_player);
    if (!playerTextDrawData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    playerTextDrawData->endSelection();
}

void PlayerExtension::beginObjectSelection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
        return;

    playerObjectData->beginSelecting();
}

void PlayerExtension::endObjectSelection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
        return;

    playerObjectData->endEditing();
}

void PlayerExtension::beginObjectEdit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
        return;

    if (info[0]->IsNullOrUndefined())
    {
        playerObjectData->endEditing();
        return;
    }

    auto v8objectId = Utils::GetIdFromV8Object(info[0]);
    if (!v8objectId.has_value())
        return;

    // global obj
    {
        auto object = NodejsComponent::getInstance()->getObjects()->get(v8objectId.value());
        if (object)
        {
            playerObjectData->beginEditing(*object);
            return;
        }
    }

    // player obj
    {
        auto object = playerObjectData->get(v8objectId.value());
        if (object)
        {
            playerObjectData->beginEditing(*object);
            return;
        }
    }
}

void PlayerExtension::endObjectEdit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
        return;

    playerObjectData->endEditing();
}

void PlayerExtension::attachCameraToObject(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
        return;

    auto v8objectId = Utils::GetIdFromV8Object(info[0]);
    if (!v8objectId.has_value())
        return;

    // global obj
    {
        auto object = NodejsComponent::getInstance()->getObjects()->get(v8objectId.value());
        if (object)
        {
            playerComponent->m_player->attachCameraToObject(*object);
            return;
        }
    }

    // player obj
    {
        auto object = playerObjectData->get(v8objectId.value());
        if (object)
        {
            playerComponent->m_player->attachCameraToObject(*object);
            return;
        }
    }
}

// ====================== accessors ======================

void PlayerExtension::getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().data()));
}

void PlayerExtension::setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8str = Utils::strV8(value);
    if (!v8str.has_value())
        return;

    playerComponent->m_player->setName(v8str.value());
}

void PlayerExtension::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getID()));
}

void PlayerExtension::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(playerComponent->m_player->getPosition()));
}

void PlayerExtension::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    playerComponent->m_player->setPosition(v8vec3.value());
}

void PlayerExtension::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Quat(playerComponent->m_player->getRotation()));
}

void PlayerExtension::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8quat = Utils::quatV8(value);
    if (!v8quat.has_value())
        return;

    playerComponent->m_player->setRotation(v8quat.value());
}

void PlayerExtension::getMoney(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerExtension::setMoney(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setMoney(v8int.value());
}

void PlayerExtension::getScore(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getScore()));
}

void PlayerExtension::setScore(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setScore(v8int.value());
}

void PlayerExtension::getSkin(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getSkin()));
}

void PlayerExtension::setSkin(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setSkin(v8int.value());
}

void PlayerExtension::getInterior(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getInterior()));
}

void PlayerExtension::setInterior(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setInterior(v8int.value());
}

void PlayerExtension::getTeam(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getTeam()));
}

void PlayerExtension::setTeam(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setInterior(v8int.value());
}

void PlayerExtension::getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getHealth()));
}

void PlayerExtension::setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setHealth(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

void PlayerExtension::getArmour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmour()));
}

void PlayerExtension::setArmour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setArmour(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

void PlayerExtension::getWeapon(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeapon()));
}

void PlayerExtension::setWeapon(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setArmedWeapon(v8int.value());
}

void PlayerExtension::getWeaponAmmo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeaponAmmo()));
}

void PlayerExtension::getDrunkLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getDrunkLevel()));
}

void PlayerExtension::setDrunkLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setDrunkLevel(v8int.value());
}

void PlayerExtension::getWantedLevel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getWantedLevel()));
}

void PlayerExtension::setWantedLevel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8int = Utils::GetIntegerFromV8Value(value);
    if (!v8int.has_value())
        return;

    playerComponent->m_player->setWantedLevel(v8int.value());
}

void PlayerExtension::getControllable(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getControllable()));
}

void PlayerExtension::setControllable(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8bool = value->ToBoolean(info.GetIsolate());
    if (v8bool.IsEmpty())
        return;

    playerComponent->m_player->setControllable(v8bool->Value());
}

void PlayerExtension::getSpawnInfo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

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

void PlayerExtension::setSpawnInfo(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

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

void PlayerExtension::getVehicle(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerVehicleData = queryExtension<IPlayerVehicleData>(playerComponent->m_player);
    if (!playerVehicleData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    if (!playerVehicleData->getVehicle())
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    auto context = info.GetIsolate()->GetCurrentContext();

    auto vehicleComponent = queryExtension<VehicleExtension>(playerVehicleData->getVehicle());
    if (!vehicleComponent)
    {
        playerVehicleData->getVehicle()->addExtension(new VehicleExtension(playerVehicleData->getVehicle(), NodejsComponent::getInstance()->getResourceManager()), true);

        vehicleComponent = queryExtension<VehicleExtension>(playerVehicleData->getVehicle());

        assert(vehicleComponent);
    }

    auto resource = NodejsComponent::getInstance()->getResourceManager()->GetResourceFromIsolate(info.GetIsolate());
    info.GetReturnValue().Set(resource->ObjectFromExtension(vehicleComponent));
}

void PlayerExtension::getIp(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    PeerAddress::AddressString addressStr;

    PeerAddress::ToString(playerComponent->m_player->getNetworkData().networkID.address, addressStr);

    info.GetReturnValue().Set(Utils::v8Str(addressStr.data()));
}

void PlayerExtension::getGhostMode(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(playerComponent->m_player->isGhostModeEnabled());
}

void PlayerExtension::setGhostMode(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8bool = Utils::GetBooleanFromV8Value(value);
    if (!v8bool.has_value())
        return;

    playerComponent->m_player->toggleGhostMode(v8bool.value());
}

void PlayerExtension::getSpecialAction(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(playerComponent->m_player->getAction());
}

void PlayerExtension::setSpecialAction(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8action = Utils::GetIntegerFromV8Value(value);
    if (!v8action.has_value())
        return;

    playerComponent->m_player->setAction((PlayerSpecialAction)v8action.value());
}

void PlayerExtension::getMenu(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerMenuData = queryExtension<IPlayerMenuData>(playerComponent->m_player);
    if (!playerMenuData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    info.GetReturnValue().Set(playerMenuData->getMenuID());
}

void PlayerExtension::setMenu(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerMenuData = queryExtension<IPlayerMenuData>(playerComponent->m_player);
    if (!playerMenuData)
        return;

    auto v8menu = Utils::GetIntegerFromV8Value(value);
    if (!v8menu.has_value())
        return;

    playerMenuData->setMenuID(v8menu.value());
}

void PlayerExtension::getDialog(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerDialogData = queryExtension<IPlayerDialogData>(playerComponent->m_player);
    if (!playerDialogData)
    {
        info.GetReturnValue().SetNull();
        return;
    }

    info.GetReturnValue().Set(playerDialogData->getActiveID());
}

void PlayerExtension::getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Colour(playerComponent->m_player->getColour()));
}

void PlayerExtension::setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8col = Utils::colourV8(value);
    if (!v8col.has_value())
        return;

    playerComponent->m_player->setColour(v8col.value());
}

void PlayerExtension::getShopName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getShopName().data()));
}

void PlayerExtension::setShopName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8str = Utils::strV8(value);
    if (!v8str.has_value())
        return;

    playerComponent->m_player->setShopName(v8str.value());
}

void PlayerExtension::getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getVirtualWorld()));
}

void PlayerExtension::setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8value = Utils::GetIntegerFromV8Value(value);
    if (!v8value.has_value())
        return;

    playerComponent->m_player->setVirtualWorld(v8value.value());
}

void PlayerExtension::getCheckpoint(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto checkpointPlayerData = queryExtension<IPlayerCheckpointData>(playerComponent->m_player);
    if (!checkpointPlayerData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    auto context = info.GetIsolate()->GetCurrentContext();

    auto checkpointComponent = queryExtension<CheckpointExtension>(playerComponent->m_player);
    if (!checkpointComponent)
    {
        playerComponent->m_player->addExtension(new CheckpointExtension(playerComponent->m_player, NodejsComponent::getInstance()->getResourceManager()), true);

        checkpointComponent = queryExtension<CheckpointExtension>(playerComponent->m_player);

        assert(checkpointComponent);
    }

    auto resource = NodejsComponent::getInstance()->getResourceManager()->GetResourceFromIsolate(info.GetIsolate());
    info.GetReturnValue().Set(resource->ObjectFromExtension(checkpointComponent));
}

void PlayerExtension::isSelecting(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerTextDrawData = queryExtension<IPlayerTextDrawData>(playerComponent->m_player);
    if (!playerTextDrawData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(playerTextDrawData->isSelecting());
}

void PlayerExtension::getSelectingObject(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(playerObjectData->selectingObject());
}

void PlayerExtension::getEditingObject(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto playerObjectData = queryExtension<IPlayerObjectData>(playerComponent->m_player);
    if (!playerObjectData)
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    info.GetReturnValue().Set(playerObjectData->editingObject());
}

void PlayerExtension::getKeys(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8obj = v8::Object::New(info.GetIsolate());
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("keys"), v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getKeyData().keys));
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("upDown"), v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getKeyData().upDown));
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), Utils::v8Str("leftRight"), v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getKeyData().leftRight));

    info.GetReturnValue().Set(v8obj);
}

void PlayerExtension::getSpectating(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    info.GetReturnValue().Set(playerComponent->m_player->getSpectateData().spectating);
}

void PlayerExtension::setSpectating(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<PlayerExtension>(info.GetIsolate(), playerComponent)) return;

    auto v8value = Utils::GetBooleanFromV8Value(value);
    if (!v8value.has_value())
        return;

    playerComponent->m_player->setSpectating(v8value.value());
}

v8::Local<v8::Object> PlayerExtension::CreateJavaScriptObject()
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
    SET_FUNCTION("setMapIcon", setMapIcon);
    SET_FUNCTION("unsetMapIcon", unsetMapIcon);
    SET_FUNCTION("sendGameText", sendGameText);
    SET_FUNCTION("getCameraPosition", getCameraPosition);
    SET_FUNCTION("getCameraLookAt", getCameraLookAt);
    SET_FUNCTION("getCameraFrontVector", getCameraFrontVector);
    SET_FUNCTION("setCameraPosition", setCameraPosition);
    SET_FUNCTION("setCameraLookAt", setCameraLookAt);
    SET_FUNCTION("setCameraBehind", setCameraBehind);
    SET_FUNCTION("beginSelection", beginSelection);
    SET_FUNCTION("endSelection", endSelection);
    SET_FUNCTION("beginObjectSelection", beginObjectSelection);
    SET_FUNCTION("endObjectSelection", endObjectSelection);
    SET_FUNCTION("beginObjectEdit", beginObjectEdit);
    SET_FUNCTION("endObjectEdit", endObjectEdit);
    SET_FUNCTION("attachCameraToObject", attachCameraToObject);

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
    SET_ACCESSOR_WITH_SETTER("colour", getColour, setColour);
    SET_ACCESSOR_WITH_SETTER("shopName", getShopName, setShopName);
    SET_ACCESSOR_WITH_SETTER("virtualWorld", getVirtualWorld, setVirtualWorld);
    SET_ACCESSOR("checkpoint", getCheckpoint);
    SET_ACCESSOR("isSelecting", isSelecting);
    SET_ACCESSOR("keys", getKeys);
    SET_ACCESSOR("selectingObject", getSelectingObject);
    SET_ACCESSOR("editingObject", getEditingObject);
    SET_ACCESSOR_WITH_SETTER("spectating", getSpectating, setSpectating);

    return v8obj;
}

void PlayerExtension::InitFunctions(Resource* resource)
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

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<PlayerExtension>(player)));
    }, v8::External::New(isolate, resource))
                                                                   .ToLocalChecked());
}

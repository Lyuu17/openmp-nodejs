
#include <Server/Components/Classes/classes.hpp>
#include <sdk.hpp>

#include "components/PlayerComponent.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

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

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    playerComponent->m_player->kick();

    info.GetReturnValue().Set(true);
}

void PlayerComponent::ban(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    std::string reason {};
    if (info[0]->IsString())
        reason = Utils::strV8(info[0]->ToString(info.GetIsolate()->GetCurrentContext()).ToLocalChecked());

    playerComponent->m_player->ban(reason.c_str());

    info.GetReturnValue().Set(true);
}

void PlayerComponent::giveMoney(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8int = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->giveMoney(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerComponent::giveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

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

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8intWeapon = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8intWeapon.IsEmpty())
        return;

    playerComponent->m_player->removeWeapon(v8intWeapon.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(true);
}

void PlayerComponent::setWeaponAmmo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

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

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto& weaponSlots = playerComponent->m_player->getWeapons();

    auto v8obj = v8::Object::New(info.GetIsolate());

    for (const auto& [wep, ammo] : weaponSlots)
        v8obj->Set(info.GetIsolate()->GetCurrentContext(), wep, v8::Number::New(info.GetIsolate(), ammo));

    info.GetReturnValue().Set(v8obj);
}

void PlayerComponent::getWeaponSlot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

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

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    playerComponent->m_player->resetWeapons();

    info.GetReturnValue().Set(true);
}

// ====================== accessors ======================

void PlayerComponent::getName(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().data()));
}

void PlayerComponent::setName(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8str = value->ToString(info.GetIsolate()->GetCurrentContext());
    if (v8str.IsEmpty())
        return;

    playerComponent->m_player->setName(Utils::strV8(v8str.ToLocalChecked()));

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().to_string().c_str()));
}

void PlayerComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getID()));
}

void PlayerComponent::getMoney(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(Utils::v8Str(playerComponent->m_player->getName().data()));
}

void PlayerComponent::setMoney(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setMoney(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), playerComponent->m_player->getMoney()));
}

void PlayerComponent::getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getHealth()));
}

void PlayerComponent::setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setHealth(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getHealth()));
}

void PlayerComponent::getArmour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmour()));
}

void PlayerComponent::setArmour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    playerComponent->m_player->setArmour(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmour()));
}

void PlayerComponent::getWeapon(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeapon()));
}

void PlayerComponent::setWeapon(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    auto v8int = value->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8int.IsEmpty())
        return;

    playerComponent->m_player->setArmedWeapon(v8int.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeapon()));
}

void PlayerComponent::getWeaponAmmo(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto playerComponent = (PlayerComponent*)info.Data().As<v8::External>()->Value();

    auto resource = ResourceManager::GetResourceFromIsolate(info.GetIsolate());
    if (!resource->DoesObjectFromExtensionExist(playerComponent))
    {
        resource->ThrowException("attempting to access a deleted component");
        return;
    }

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), playerComponent->m_player->getArmedWeaponAmmo()));
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
    SET_ACCESSOR_WITH_SETTER("money", getMoney, setMoney);
    SET_ACCESSOR_WITH_SETTER("health", getHealth, setHealth);
    SET_ACCESSOR_WITH_SETTER("armour", getArmour, setArmour);
    SET_ACCESSOR_WITH_SETTER("weapon", getWeapon, setWeapon);
    SET_ACCESSOR("weaponAmmo", getWeaponAmmo);

    return v8obj;
}

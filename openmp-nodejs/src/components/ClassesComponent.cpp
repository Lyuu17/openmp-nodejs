
#include <Server/Components/Classes/classes.hpp>
#include <sdk.hpp>

#include "components/ClassesComponent.hpp"

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

void ClassesComponent::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("addPlayerClass"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8skin    = Utils::GetIntegerFromV8Value(info[0]);
        auto v8team    = Utils::GetIntegerFromV8Value(info[1]);
        auto v8spawn   = Utils::vector3V8(info[2]);
        auto v8angle   = Utils::GetDoubleFromV8Value(info[3]);
        auto v8weapons = info[4];

        if (!v8skin.has_value() || !v8team.has_value() || !v8spawn.has_value() || !v8angle.has_value() || !v8weapons.IsEmpty() && !v8weapons->IsArray() && !v8weapons->IsUndefined())
            return;

        int     skin  = v8skin.value();
        int     team  = v8team.value();
        Vector3 spawn = v8spawn.value();
        float   angle = v8angle.value_or(0.0f);

        WeaponSlots weaponSlots = Utils::GetWeaponSlotsDataFromV8Object(v8weapons).value_or(WeaponSlots {});

        NodejsComponent::getInstance()->getClasses()->create(skin, team, spawn, angle, weaponSlots);
    }, v8::External::New(isolate, resource))
                                                                        .ToLocalChecked());
}

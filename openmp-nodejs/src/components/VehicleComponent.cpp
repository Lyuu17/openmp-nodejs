
#include <sdk.hpp>

#include "components/VehicleComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_VEHICLE_HAS_COMPONENT(vehicle, component)                                                           \
    if (!queryExtension<component>(vehicle))                                                                       \
    {                                                                                                              \
        vehicle->addExtension(new component(vehicle, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<component>(vehicle));                                                                \
    }

VehicleComponent::VehicleComponent(IVehicle* vehicle, ResourceManager* resourceManager)
    : m_vehicle(vehicle)
    , m_resourceManager(resourceManager)
{
}

VehicleComponent::~VehicleComponent()
{
}

void VehicleComponent::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void VehicleComponent::reset()
{
}

// ====================== methods ======================

void VehicleComponent::repair(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    vehicleComponent->m_vehicle->repair();
}

void VehicleComponent::putPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8player = info[0]->ToObject(info.GetIsolate()->GetCurrentContext());
    auto v8seat   = info[1]->ToInteger(info.GetIsolate()->GetCurrentContext());
    if (v8player.IsEmpty() || v8seat.IsEmpty())
        return;

    auto playerId = Utils::GetIdFromV8Object(v8player);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());

    vehicleComponent->m_vehicle->putPlayer(*player, v8seat.ToLocalChecked()->IntegerValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

// ====================== accessors ======================

void VehicleComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getID()));
}

void VehicleComponent::getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getHealth()));
}

void VehicleComponent::setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    vehicleComponent->m_vehicle->setHealth(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

void VehicleComponent::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    info.GetReturnValue().Set(Utils::v8Vector3(vehicleComponent->m_vehicle->getPosition()));
}

void VehicleComponent::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    vehicleComponent->m_vehicle->setPosition(v8vec3.value());
}

void VehicleComponent::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    info.GetReturnValue().Set(Utils::v8Quat(vehicleComponent->m_vehicle->getRotation()));
}

void VehicleComponent::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8quat = Utils::quatV8(value);
    if (!v8quat.has_value())
        return;

    vehicleComponent->m_vehicle->setRotation(v8quat.value());
}

void VehicleComponent::getDriver(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto driver = queryExtension<PlayerComponent>(vehicleComponent->m_vehicle->getDriver());

    info.GetReturnValue().Set(driver->CreateJavaScriptObject());
}

void VehicleComponent::getPlate(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    info.GetReturnValue().Set(Utils::v8Str(vehicleComponent->m_vehicle->getPlate().data()));
}

void VehicleComponent::setPlate(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8str = value->ToString(info.GetIsolate()->GetCurrentContext());
    if (v8str.IsEmpty())
        return;

    vehicleComponent->m_vehicle->setPlate(Utils::strV8(v8str.ToLocalChecked()));
}

void VehicleComponent::getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getModel()));
}

void VehicleComponent::getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8color = v8::Array::New(info.GetIsolate(), 2);
    v8color->Set(info.GetIsolate()->GetCurrentContext(), 0, v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getColour().first));
    v8color->Set(info.GetIsolate()->GetCurrentContext(), 1, v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getColour().second));

    info.GetReturnValue().Set(v8color);
}

void VehicleComponent::setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleComponent*)info.Data().As<v8::External>()->Value();

    Utils::CheckExtensionExist<VehicleComponent>(info.GetIsolate(), vehicleComponent);

    auto v8obj = value->ToObject(info.GetIsolate()->GetCurrentContext());
    if (v8obj.IsEmpty())
        return;

    auto v8color1 = Utils::GetIntegerFromV8Value(v8obj.ToLocalChecked()->Get(info.GetIsolate()->GetCurrentContext(), 0));
    auto v8color2 = Utils::GetIntegerFromV8Value(v8obj.ToLocalChecked()->Get(info.GetIsolate()->GetCurrentContext(), 1));

    vehicleComponent->m_vehicle->setColour(v8color1.value_or(0), v8color2.value_or(0));
}

v8::Local<v8::Object> VehicleComponent::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("repair", repair);
    SET_FUNCTION("putPlayer", putPlayer);

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("health", getHealth, setHealth);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("rotation", getRotation, setRotation);
    SET_ACCESSOR("driver", getDriver);
    SET_ACCESSOR_WITH_SETTER("plate", getPlate, setPlate);
    SET_ACCESSOR("model", getModel);
    SET_ACCESSOR_WITH_SETTER("colour", getColour, setColour);

    return v8obj;
}

void VehicleComponent::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("getVehicle"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8int = Utils::GetIntegerFromV8Value(info[0]);
        if (!v8int.has_value())
            return;

        auto vehicle = NodejsComponent::getInstance()->getVehicles()->get(v8int.value());
        if (!vehicle)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        ENSURE_VEHICLE_HAS_COMPONENT(vehicle, VehicleComponent);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle)));
    }, v8::External::New(isolate, resource))
                                                                    .ToLocalChecked());

    context->Global()->Set(context, Utils::v8Str("createVehicle"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8isStatic     = info[0]->ToBoolean(info.GetIsolate());
        auto v8modelId      = Utils::GetIntegerFromV8Value(info[1]);
        auto v8position     = Utils::vector3V8(info[2]);
        auto v8angle        = Utils::GetDoubleFromV8Value(info[3]);
        auto v8color1       = Utils::GetIntegerFromV8Value(info[4]);
        auto v8color2       = Utils::GetIntegerFromV8Value(info[5]);
        auto v8respawnDelay = Utils::GetIntegerFromV8Value(info[6]);
        auto v8addSiren     = info[7]->ToBoolean(info.GetIsolate());

        if (v8isStatic.IsEmpty() || !v8modelId.has_value() || !v8position.has_value())
            return;

        bool    isStatic     = v8isStatic->ToBoolean(info.GetIsolate())->Value();
        int     modelId      = v8modelId.value();
        Vector3 position     = v8position.value();
        float   angle        = v8angle.value_or(0.0f);
        int     color1       = v8color1.value_or(-1);
        int     color2       = v8color2.value_or(-1);
        int     respawnDelay = v8respawnDelay.value_or(300);
        bool    addSiren     = !v8addSiren.IsEmpty() ? v8addSiren->ToBoolean(info.GetIsolate())->Value() : false;

        auto vehicle = NodejsComponent::getInstance()->getVehicles()->create(isStatic, modelId, position, angle, color1, color2, Seconds { respawnDelay }, addSiren);
        if (!vehicle)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        ENSURE_VEHICLE_HAS_COMPONENT(vehicle, VehicleComponent);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle)));
    }, v8::External::New(isolate, resource))
                                                                       .ToLocalChecked());
}

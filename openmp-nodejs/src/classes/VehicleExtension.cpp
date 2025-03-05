
#include <Server/Components/Vehicles/vehicle_seats.hpp>
#include <sdk.hpp>

#include "classes/VehicleExtension.hpp"
#include "classes/PlayerExtension.hpp"
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

VehicleExtension::VehicleExtension(IVehicle* vehicle, ResourceManager* resourceManager)
    : m_vehicle(vehicle)
    , m_resourceManager(resourceManager)
{
}

VehicleExtension::~VehicleExtension()
{
}

void VehicleExtension::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void VehicleExtension::reset()
{
}

// ====================== methods ======================

void VehicleExtension::repair(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    vehicleComponent->m_vehicle->repair();
}

void VehicleExtension::putPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    auto v8seat   = Utils::GetIntegerFromV8Value(info[1]);
    if (!playerId.has_value() || !v8seat.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());

    vehicleComponent->m_vehicle->putPlayer(*player, v8seat.value());
}

void VehicleExtension::addComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8comp = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8comp.has_value())
        return;

    vehicleComponent->m_vehicle->addComponent(v8comp.value());
}

void VehicleExtension::getComponentInSlot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8slot = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8slot.has_value())
        return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), vehicleComponent->m_vehicle->getComponentInSlot(v8slot.value())));
}

void VehicleExtension::removeComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8comp = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8comp.has_value())
        return;

    vehicleComponent->m_vehicle->removeComponent(v8comp.value());
}

void VehicleExtension::getParam(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8paramId = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8paramId.has_value())
        return;

    auto& params = vehicleComponent->m_vehicle->getParams();

    switch ((VehicleParamType)v8paramId.value())
    {
    case VehicleParamType::DOORS: info.GetReturnValue().Set(params.doors); break;
    case VehicleParamType::BONNET: info.GetReturnValue().Set(params.bonnet); break;
    case VehicleParamType::BOOT: info.GetReturnValue().Set(params.boot); break;
    case VehicleParamType::OBJECTIVE: info.GetReturnValue().Set(params.objective); break;
    case VehicleParamType::SIREN: info.GetReturnValue().Set(params.siren); break;
    case VehicleParamType::DOOR_DRIVER: info.GetReturnValue().Set(params.doorDriver); break;
    case VehicleParamType::DOOR_PASSENGER: info.GetReturnValue().Set(params.doorPassenger); break;
    case VehicleParamType::DOOR_BACKLEFT: info.GetReturnValue().Set(params.doorBackLeft); break;
    case VehicleParamType::DOOR_BACKRIGHT: info.GetReturnValue().Set(params.doorBackRight); break;
    case VehicleParamType::WINDOW_DRIVER: info.GetReturnValue().Set(params.windowDriver); break;
    case VehicleParamType::WINDOW_PASSENGER: info.GetReturnValue().Set(params.windowPassenger); break;
    case VehicleParamType::WINDOW_BACKLEFT: info.GetReturnValue().Set(params.windowBackLeft); break;
    case VehicleParamType::WINDOW_BACKRIGHT: info.GetReturnValue().Set(params.windowBackRight); break;
    default:
        break;
    }
}

void VehicleExtension::setParam(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8paramId = Utils::GetIntegerFromV8Value(info[0]);
    if (!v8paramId.has_value())
        return;

    auto v8value = Utils::GetIntegerFromV8Value(info[1]);
    if (!v8value.has_value())
        return;

    VehicleParams params { vehicleComponent->m_vehicle->getParams() };

    switch ((VehicleParamType)v8paramId.value())
    {
    case VehicleParamType::DOORS: params.doors = v8value.value(); break;
    case VehicleParamType::BONNET: params.bonnet = v8value.value(); break;
    case VehicleParamType::BOOT: params.boot = v8value.value(); break;
    case VehicleParamType::OBJECTIVE: params.objective = v8value.value(); break;
    case VehicleParamType::SIREN: params.siren = v8value.value(); break;
    case VehicleParamType::DOOR_DRIVER: params.doorDriver = v8value.value(); break;
    case VehicleParamType::DOOR_PASSENGER: params.doorPassenger = v8value.value(); break;
    case VehicleParamType::DOOR_BACKLEFT: params.doorBackLeft = v8value.value(); break;
    case VehicleParamType::DOOR_BACKRIGHT: params.doorBackRight = v8value.value(); break;
    case VehicleParamType::WINDOW_DRIVER: params.windowDriver = v8value.value(); break;
    case VehicleParamType::WINDOW_PASSENGER: params.windowPassenger = v8value.value(); break;
    case VehicleParamType::WINDOW_BACKLEFT: params.windowBackLeft = v8value.value(); break;
    case VehicleParamType::WINDOW_BACKRIGHT: params.windowBackRight = v8value.value(); break;
    default:
        break;
    }

    auto playerId = Utils::GetIdFromV8Object(info[2]);
    if (!playerId.has_value())
    {
        vehicleComponent->m_vehicle->setParams(params);
    }
    else
    {
        auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());

        if (player) vehicleComponent->m_vehicle->setParamsForPlayer(*player, params);
    }
}

// ====================== accessors ======================

void VehicleExtension::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getID()));
}

void VehicleExtension::getHealth(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getHealth()));
}

void VehicleExtension::setHealth(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8number = value->ToNumber(info.GetIsolate()->GetCurrentContext());
    if (v8number.IsEmpty())
        return;

    vehicleComponent->m_vehicle->setHealth(v8number.ToLocalChecked()->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked());
}

void VehicleExtension::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(vehicleComponent->m_vehicle->getPosition()));
}

void VehicleExtension::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8vec3 = Utils::vector3V8(value);
    if (!v8vec3.has_value())
        return;

    vehicleComponent->m_vehicle->setPosition(v8vec3.value());
}

void VehicleExtension::getRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(Utils::v8Quat(vehicleComponent->m_vehicle->getRotation()));
}

void VehicleExtension::setRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8quat = Utils::quatV8(value);
    if (!v8quat.has_value())
        return;

    vehicleComponent->m_vehicle->setRotation(v8quat.value());
}

void VehicleExtension::getDriver(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    if (!vehicleComponent->m_vehicle->getDriver())
    {
        info.GetReturnValue().SetUndefined();
        return;
    }

    auto driver = queryExtension<PlayerExtension>(vehicleComponent->m_vehicle->getDriver());

    info.GetReturnValue().Set(driver->CreateJavaScriptObject());
}

void VehicleExtension::getPassengers(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto  maxPassengers = Impl::getVehiclePassengerSeats(vehicleComponent->m_vehicle->getModel());
    auto& passengers    = vehicleComponent->m_vehicle->getPassengers();

    auto v8obj = v8::Array::New(info.GetIsolate(), maxPassengers);
    for (const auto& passenger : passengers)
    {
        auto passengerVehicleData = queryExtension<IPlayerVehicleData>(passenger);
        if (!passengerVehicleData)
        {
            info.GetReturnValue().SetNull();
            continue;
        }

        auto passengerComponent = queryExtension<PlayerExtension>(passenger);
        v8obj->Set(info.GetIsolate()->GetCurrentContext(), passengerVehicleData->getSeat(), passengerComponent->CreateJavaScriptObject());
    }

    info.GetReturnValue().Set(v8obj);
}

void VehicleExtension::getPlate(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(Utils::v8Str(vehicleComponent->m_vehicle->getPlate().data()));
}

void VehicleExtension::setPlate(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8str = Utils::strV8(value);
    if (!v8str.has_value())
        return;

    vehicleComponent->m_vehicle->setPlate(v8str.value());
}

void VehicleExtension::getModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getModel()));
}

void VehicleExtension::getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8color = v8::Array::New(info.GetIsolate(), 2);
    v8color->Set(info.GetIsolate()->GetCurrentContext(), 0, v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getColour().first));
    v8color->Set(info.GetIsolate()->GetCurrentContext(), 1, v8::Number::New(info.GetIsolate(), vehicleComponent->m_vehicle->getColour().second));

    info.GetReturnValue().Set(v8color);
}

void VehicleExtension::setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8obj = value->ToObject(info.GetIsolate()->GetCurrentContext());
    if (v8obj.IsEmpty())
        return;

    auto v8color1 = Utils::GetIntegerFromV8Value(v8obj.ToLocalChecked()->Get(info.GetIsolate()->GetCurrentContext(), 0));
    auto v8color2 = Utils::GetIntegerFromV8Value(v8obj.ToLocalChecked()->Get(info.GetIsolate()->GetCurrentContext(), 1));

    vehicleComponent->m_vehicle->setColour(v8color1.value_or(0), v8color2.value_or(0));
}

void VehicleExtension::getVirtualWorld(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), vehicleComponent->m_vehicle->getVirtualWorld()));
}

void VehicleExtension::setVirtualWorld(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8value = Utils::GetIntegerFromV8Value(value);
    if (!v8value.has_value())
        return;

    vehicleComponent->m_vehicle->setVirtualWorld(v8value.value());
}

void VehicleExtension::getEngine(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto& params = vehicleComponent->m_vehicle->getParams();

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), params.engine));
}

void VehicleExtension::setEngine(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8value = Utils::GetBooleanFromV8Value(value);
    if (!v8value.has_value())
        return;

    VehicleParams params { vehicleComponent->m_vehicle->getParams() };

    params.engine = v8value.value();

    vehicleComponent->m_vehicle->setParams(params);
}

void VehicleExtension::getLights(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto& params = vehicleComponent->m_vehicle->getParams();

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), params.lights));
}

void VehicleExtension::setLights(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8value = Utils::GetBooleanFromV8Value(value);
    if (!v8value.has_value())
        return;

    VehicleParams params { vehicleComponent->m_vehicle->getParams() };

    params.lights = v8value.value();

    vehicleComponent->m_vehicle->setParams(params);
}

void VehicleExtension::getAlarm(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto& params = vehicleComponent->m_vehicle->getParams();

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), params.alarm));
}

void VehicleExtension::setAlarm(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto vehicleComponent = (VehicleExtension*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<VehicleExtension>(info.GetIsolate(), vehicleComponent)) return;

    auto v8value = Utils::GetBooleanFromV8Value(value);
    if (!v8value.has_value())
        return;

    VehicleParams params { vehicleComponent->m_vehicle->getParams() };

    params.alarm = v8value.value();

    vehicleComponent->m_vehicle->setParams(params);
}

v8::Local<v8::Object> VehicleExtension::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("repair", repair);
    SET_FUNCTION("putPlayer", putPlayer);
    SET_FUNCTION("addComponent", addComponent);
    SET_FUNCTION("getComponentInSlot", getComponentInSlot);
    SET_FUNCTION("removeComponent", removeComponent);
    SET_FUNCTION("getParam", getParam);
    SET_FUNCTION("setParam", setParam);

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("health", getHealth, setHealth);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("rotation", getRotation, setRotation);
    SET_ACCESSOR("driver", getDriver);
    SET_ACCESSOR("passengers", getPassengers);
    SET_ACCESSOR_WITH_SETTER("plate", getPlate, setPlate);
    SET_ACCESSOR("model", getModel);
    SET_ACCESSOR_WITH_SETTER("colour", getColour, setColour);
    SET_ACCESSOR_WITH_SETTER("virtualWorld", getVirtualWorld, setVirtualWorld);
    SET_ACCESSOR_WITH_SETTER("engine", getEngine, setEngine);
    SET_ACCESSOR_WITH_SETTER("lights", getLights, setLights);
    SET_ACCESSOR_WITH_SETTER("alarm", getAlarm, setAlarm);

    return v8obj;
}

void VehicleExtension::InitFunctions(Resource* resource)
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

        ENSURE_VEHICLE_HAS_COMPONENT(vehicle, VehicleExtension);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle)));
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

        ENSURE_VEHICLE_HAS_COMPONENT(vehicle, VehicleExtension);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle)));
    }, v8::External::New(isolate, resource))
                                                                       .ToLocalChecked());
}

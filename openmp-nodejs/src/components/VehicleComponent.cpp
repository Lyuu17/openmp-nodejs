
#include <Server/Components/Classes/classes.hpp>
#include <sdk.hpp>

#include "components/VehicleComponent.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define CHECK_EXTENSION_EXIST(isolate, component)                             \
    auto resource = ResourceManager::GetResourceFromIsolate(isolate);         \
    if (!resource->DoesObjectFromExtensionExist(component))                   \
    {                                                                         \
        resource->ThrowException("attempting to access a deleted component"); \
        return;                                                               \
    }

VehicleComponent::VehicleComponent(IVehicle* vehicle)
    : m_vehicle(vehicle)
{
}

VehicleComponent::~VehicleComponent()
{
}

void VehicleComponent::freeExtension()
{
    ResourceManager::Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void VehicleComponent::reset()
{
}

// ====================== methods ======================

// ====================== accessors ======================

v8::Local<v8::Object> VehicleComponent::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

#define SET_ACCESSOR(f, getter) v8obj->SetAccessor(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetAccessor(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    return v8obj;
}

void VehicleComponent::InitFunctions()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("getVehicle"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8int = info[0]->ToInteger(info.GetIsolate()->GetCurrentContext());
        if (v8int.IsEmpty())
            return;

        auto vehicle = NodejsComponent::getInstance()->getVehicles()->get(v8int.ToLocalChecked()->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked());
        if (!vehicle)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        auto vehicleExtension = queryExtension<VehicleComponent>(vehicle);
        if (!vehicleExtension)
        {
            vehicle->addExtension(new VehicleComponent(vehicle), true);

            vehicleExtension = queryExtension<VehicleComponent>(vehicle);

            assert(vehicleExtension);
        }

        info.GetReturnValue().Set(vehicleExtension->CreateJavaScriptObject());
    }).ToLocalChecked());
}

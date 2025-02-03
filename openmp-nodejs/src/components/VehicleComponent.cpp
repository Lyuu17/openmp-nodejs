
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

void VehicleComponent::InitFunctions(Resource* resource)
{
}

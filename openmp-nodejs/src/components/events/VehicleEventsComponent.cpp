
#include "components/events/VehicleEventsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/VehicleComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

VehicleEventsComponent::VehicleEventsComponent(ICore* core, IVehiclesComponent* vehicles)
    : m_core(core)
    , m_vehicles(vehicles)
{
    if (m_vehicles)
    {
        m_vehicles->getEventDispatcher().addEventHandler(this);
    }
}

VehicleEventsComponent::~VehicleEventsComponent()
{
    if (m_core)
    {
    }

    if (m_vehicles)
    {
        m_vehicles->getEventDispatcher().removeEventHandler(this);
    }
}

void VehicleEventsComponent::freeExtension()
{
    delete this;
}

void VehicleEventsComponent::reset()
{
}

// =========== events

void VehicleEventsComponent::onVehicleStreamIn(IVehicle& vehicle, IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onVehicleStreamIn", { v8objVehicle, v8objPlayer });
    });
}

void VehicleEventsComponent::onVehicleStreamOut(IVehicle& vehicle, IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onVehicleStreamOut", { v8objVehicle, v8objPlayer });
    });
}

void VehicleEventsComponent::onVehicleDeath(IVehicle& vehicle, IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onVehicleDeath", { v8objVehicle, v8objPlayer });
    });
}

void VehicleEventsComponent::onPlayerEnterVehicle(IPlayer& player, IVehicle& vehicle, bool passenger)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Object>  v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Boolean> v8passenger  = v8::Boolean::New(resource->m_isolate, passenger);

        resource->Emit("onPlayerEnterVehicle", { v8objPlayer, v8objVehicle, v8passenger });
    });
}

void VehicleEventsComponent::onPlayerExitVehicle(IPlayer& player, IVehicle& vehicle)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerExitVehicle", { v8objPlayer, v8objVehicle });
    });
}

void VehicleEventsComponent::onVehicleDamageStatusUpdate(IVehicle& vehicle, IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onVehicleDamageStatusUpdate", { v8objVehicle, v8objPlayer });
    });
}

bool VehicleEventsComponent::onVehiclePaintJob(IPlayer& player, IVehicle& vehicle, int paintJob)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Number> v8paintJob   = v8::Number::New(resource->m_isolate, paintJob);

        resource->Emit("onVehiclePaintJob", { cancellableEventObj, v8objPlayer, v8objVehicle, v8paintJob });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsComponent::onVehicleMod(IPlayer& player, IVehicle& vehicle, int component)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Number> v8component  = v8::Number::New(resource->m_isolate, component);

        resource->Emit("onVehicleMod", { cancellableEventObj, v8objPlayer, v8objVehicle, v8component });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsComponent::onVehicleRespray(IPlayer& player, IVehicle& vehicle, int colour1, int colour2)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Number> v8color1     = v8::Number::New(resource->m_isolate, colour1);
        v8::Local<v8::Number> v8color2     = v8::Number::New(resource->m_isolate, colour2);

        resource->Emit("onVehicleRespray", { cancellableEventObj, v8objPlayer, v8objVehicle, v8color1, v8color2 });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

void VehicleEventsComponent::onEnterExitModShop(IPlayer& player, bool enterexit, int interiorID)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Boolean> v8enterexit  = v8::Boolean::New(resource->m_isolate, enterexit);
        v8::Local<v8::Number>  v8interiorId = v8::Number::New(resource->m_isolate, interiorID);

        resource->Emit("onEnterExitModShop", { v8objPlayer, v8enterexit, v8interiorId });
    });
}

void VehicleEventsComponent::onVehicleSpawn(IVehicle& vehicle)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));

        resource->Emit("onVehicleSpawn", { v8objVehicle });
    });
}

bool VehicleEventsComponent::onUnoccupiedVehicleUpdate(IVehicle& vehicle, IPlayer& player, UnoccupiedVehicleUpdate const updateData) { return true; }

bool VehicleEventsComponent::onTrailerUpdate(IPlayer& player, IVehicle& trailer)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objTrailer = resource->ObjectFromExtension(queryExtension<VehicleComponent>(trailer));

        resource->Emit("onTrailerUpdate", { cancellableEventObj, v8objPlayer, v8objTrailer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsComponent::onVehicleSirenStateChange(IPlayer& player, IVehicle& vehicle, uint8_t sirenState)
{
    bool cancelled = false;

    ResourceManager::Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));
        v8::Local<v8::Number> v8sirenState = v8::Number::New(resource->m_isolate, sirenState);

        resource->Emit("onVehicleSirenStateChange", { cancellableEventObj, v8objPlayer, v8sirenState });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

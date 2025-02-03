
#include "components/events/VehicleEventsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "components/VehicleComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"

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

bool VehicleEventsComponent::onVehiclePaintJob(IPlayer& player, IVehicle& vehicle, int paintJob) { return true; }
bool VehicleEventsComponent::onVehicleMod(IPlayer& player, IVehicle& vehicle, int component) { return true; }
bool VehicleEventsComponent::onVehicleRespray(IPlayer& player, IVehicle& vehicle, int colour1, int colour2) { return true; }
void VehicleEventsComponent::onEnterExitModShop(IPlayer& player, bool enterexit, int interiorID) {}

void VehicleEventsComponent::onVehicleSpawn(IVehicle& vehicle)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleComponent>(vehicle));

        resource->Emit("onVehicleSpawn", { v8objVehicle });
    });
}

bool VehicleEventsComponent::onUnoccupiedVehicleUpdate(IVehicle& vehicle, IPlayer& player, UnoccupiedVehicleUpdate const updateData) { return true; }
bool VehicleEventsComponent::onTrailerUpdate(IPlayer& player, IVehicle& trailer) { return true; }
bool VehicleEventsComponent::onVehicleSirenStateChange(IPlayer& player, IVehicle& vehicle, uint8_t sirenState) { return true; }

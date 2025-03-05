
#include "events/VehicleEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"
#include "classes/VehicleExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

VehicleEventsExtension::VehicleEventsExtension(ICore* core, IVehiclesComponent* vehicles, ResourceManager* resourceManager)
    : m_core(core)
    , m_vehicles(vehicles)
    , m_resourceManager(resourceManager)
{
    if (m_vehicles)
    {
        m_vehicles->getEventDispatcher().addEventHandler(this);
    }
}

VehicleEventsExtension::~VehicleEventsExtension()
{
    if (m_core)
    {
    }

    if (m_vehicles)
    {
        // crash
        // m_vehicles->getEventDispatcher().removeEventHandler(this);
    }
}

void VehicleEventsExtension::freeExtension()
{
    delete this;
}

void VehicleEventsExtension::reset()
{
}

// =========== events

void VehicleEventsExtension::onVehicleStreamIn(IVehicle& vehicle, IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onVehicleStreamIn", { v8objVehicle, v8objPlayer });
    });
}

void VehicleEventsExtension::onVehicleStreamOut(IVehicle& vehicle, IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onVehicleStreamOut", { v8objVehicle, v8objPlayer });
    });
}

void VehicleEventsExtension::onVehicleDeath(IVehicle& vehicle, IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onVehicleDeath", { v8objVehicle, v8objPlayer });
    });
}

void VehicleEventsExtension::onPlayerEnterVehicle(IPlayer& player, IVehicle& vehicle, bool passenger)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object>  v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Boolean> v8passenger  = v8::Boolean::New(resource->m_isolate, passenger);

        resource->Emit("onPlayerEnterVehicle", { v8objPlayer, v8objVehicle, v8passenger });
    });
}

void VehicleEventsExtension::onPlayerExitVehicle(IPlayer& player, IVehicle& vehicle)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerExitVehicle", { v8objPlayer, v8objVehicle });
    });
}

void VehicleEventsExtension::onVehicleDamageStatusUpdate(IVehicle& vehicle, IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onVehicleDamageStatusUpdate", { v8objVehicle, v8objPlayer });
    });
}

bool VehicleEventsExtension::onVehiclePaintJob(IPlayer& player, IVehicle& vehicle, int paintJob)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Number> v8paintJob   = v8::Number::New(resource->m_isolate, paintJob);

        resource->Emit("onVehiclePaintJob", { cancellableEventObj, v8objPlayer, v8objVehicle, v8paintJob });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsExtension::onVehicleMod(IPlayer& player, IVehicle& vehicle, int component)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Number> v8component  = v8::Number::New(resource->m_isolate, component);

        resource->Emit("onVehicleMod", { cancellableEventObj, v8objPlayer, v8objVehicle, v8component });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsExtension::onVehicleRespray(IPlayer& player, IVehicle& vehicle, int colour1, int colour2)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Number> v8color1     = v8::Number::New(resource->m_isolate, colour1);
        v8::Local<v8::Number> v8color2     = v8::Number::New(resource->m_isolate, colour2);

        resource->Emit("onVehicleRespray", { cancellableEventObj, v8objPlayer, v8objVehicle, v8color1, v8color2 });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

void VehicleEventsExtension::onEnterExitModShop(IPlayer& player, bool enterexit, int interiorID)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Boolean> v8enterexit  = v8::Boolean::New(resource->m_isolate, enterexit);
        v8::Local<v8::Number>  v8interiorId = v8::Number::New(resource->m_isolate, interiorID);

        resource->Emit("onEnterExitModShop", { v8objPlayer, v8enterexit, v8interiorId });
    });
}

void VehicleEventsExtension::onVehicleSpawn(IVehicle& vehicle)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));

        resource->Emit("onVehicleSpawn", { v8objVehicle });
    });
}

bool VehicleEventsExtension::onUnoccupiedVehicleUpdate(IVehicle& vehicle, IPlayer& player, UnoccupiedVehicleUpdate const updateData)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        auto context = resource->m_isolate->GetCurrentContext();

        auto v8updateData = v8::Object::New(resource->m_isolate);
        v8updateData->Set(context, Utils::v8Str("seat"), v8::Number::New(resource->m_isolate, updateData.seat));
        v8updateData->Set(context, Utils::v8Str("position"), Utils::v8Vector3(updateData.position));
        v8updateData->Set(context, Utils::v8Str("velocity"), Utils::v8Vector3(updateData.velocity));

        resource->Emit("onUnoccupiedVehicleUpdate", { cancellableEventObj, v8objVehicle, v8objPlayer, v8updateData });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsExtension::onTrailerUpdate(IPlayer& player, IVehicle& trailer)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objTrailer = resource->ObjectFromExtension(queryExtension<VehicleExtension>(trailer));

        resource->Emit("onTrailerUpdate", { cancellableEventObj, v8objPlayer, v8objTrailer });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

bool VehicleEventsExtension::onVehicleSirenStateChange(IPlayer& player, IVehicle& vehicle, uint8_t sirenState)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object> v8objPlayer  = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Object> v8objVehicle = resource->ObjectFromExtension(queryExtension<VehicleExtension>(vehicle));
        v8::Local<v8::Number> v8sirenState = v8::Number::New(resource->m_isolate, sirenState);

        resource->Emit("onVehicleSirenStateChange", { cancellableEventObj, v8objPlayer, v8sirenState });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

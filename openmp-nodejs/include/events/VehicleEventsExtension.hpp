
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Vehicles/vehicles.hpp>
#include <sdk.hpp>

struct VehicleEventsExtension final : public IExtension,
                                      public VehicleEventHandler
{
    PROVIDE_EXT_UID(0x48875A29CFD4F6EF);

    ICore*              m_core            = nullptr;
    IVehiclesComponent* m_vehicles        = nullptr;
    ResourceManager*    m_resourceManager = nullptr;

public:
    VehicleEventsExtension(ICore*, IVehiclesComponent*, ResourceManager*);
    ~VehicleEventsExtension();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onVehicleStreamIn(IVehicle& vehicle, IPlayer& player) override;
    void onVehicleStreamOut(IVehicle& vehicle, IPlayer& player) override;
    void onVehicleDeath(IVehicle& vehicle, IPlayer& player) override;
    void onPlayerEnterVehicle(IPlayer& player, IVehicle& vehicle, bool passenger) override;
    void onPlayerExitVehicle(IPlayer& player, IVehicle& vehicle) override;
    void onVehicleDamageStatusUpdate(IVehicle& vehicle, IPlayer& player) override;
    bool onVehiclePaintJob(IPlayer& player, IVehicle& vehicle, int paintJob) override;
    bool onVehicleMod(IPlayer& player, IVehicle& vehicle, int component) override;
    bool onVehicleRespray(IPlayer& player, IVehicle& vehicle, int colour1, int colour2) override;
    void onEnterExitModShop(IPlayer& player, bool enterexit, int interiorID) override;
    void onVehicleSpawn(IVehicle& vehicle) override;
    bool onUnoccupiedVehicleUpdate(IVehicle& vehicle, IPlayer& player, UnoccupiedVehicleUpdate const updateData) override;
    bool onTrailerUpdate(IPlayer& player, IVehicle& trailer) override;
    bool onVehicleSirenStateChange(IPlayer& player, IVehicle& vehicle, uint8_t sirenState) override;
};

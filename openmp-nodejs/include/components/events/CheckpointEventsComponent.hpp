
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Checkpoints/checkpoints.hpp>
#include <sdk.hpp>

struct CheckpointEventsComponent final : public IExtension,
                                         public PlayerCheckpointEventHandler
{
    PROVIDE_EXT_UID(0xB3B84E861DC8C6B2);

    ICore*                 m_core            = nullptr;
    ICheckpointsComponent* m_checkpoints     = nullptr;
    ResourceManager*       m_resourceManager = nullptr;

public:
    CheckpointEventsComponent(ICore*, ICheckpointsComponent* checkpoints, ResourceManager* resourceManager);
    ~CheckpointEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onPlayerEnterCheckpoint(IPlayer& player) override;
    void onPlayerLeaveCheckpoint(IPlayer& player) override;
    void onPlayerEnterRaceCheckpoint(IPlayer& player) override;
    void onPlayerLeaveRaceCheckpoint(IPlayer& player) override;
};


#include "events/CheckpointEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

CheckpointEventsExtension::CheckpointEventsExtension(ICore* core, ICheckpointsComponent* checkpoints, ResourceManager* resourceManager)
    : m_core(core)
    , m_checkpoints(checkpoints)
    , m_resourceManager(resourceManager)
{
    if (checkpoints)
    {
        checkpoints->getEventDispatcher().addEventHandler(this);
    }
}

CheckpointEventsExtension::~CheckpointEventsExtension()
{
    if (m_core)
    {
    }
}

void CheckpointEventsExtension::freeExtension()
{
    delete this;
}

void CheckpointEventsExtension::reset()
{
}

// =========== events

void CheckpointEventsExtension::onPlayerEnterCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerEnterCheckpoint", { v8objPlayer });
    });
};

void CheckpointEventsExtension::onPlayerLeaveCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerLeaveCheckpoint", { v8objPlayer });
    });
}

void CheckpointEventsExtension::onPlayerEnterRaceCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerEnterRaceCheckpoint", { v8objPlayer });
    });
}

void CheckpointEventsExtension::onPlayerLeaveRaceCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerLeaveRaceCheckpoint", { v8objPlayer });
    });
}

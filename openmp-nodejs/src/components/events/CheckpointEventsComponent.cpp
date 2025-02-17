
#include "components/events/CheckpointEventsComponent.hpp"
#include "components/PlayerComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

CheckpointEventsComponent::CheckpointEventsComponent(ICore* core, ICheckpointsComponent* checkpoints, ResourceManager* resourceManager)
    : m_core(core)
    , m_checkpoints(checkpoints)
    , m_resourceManager(resourceManager)
{
    if (checkpoints)
    {
        checkpoints->getEventDispatcher().addEventHandler(this);
    }
}

CheckpointEventsComponent::~CheckpointEventsComponent()
{
    if (m_core)
    {
    }
}

void CheckpointEventsComponent::freeExtension()
{
    delete this;
}

void CheckpointEventsComponent::reset()
{
}

// =========== events

void CheckpointEventsComponent::onPlayerEnterCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerEnterCheckpoint", { v8objPlayer });
    });
};

void CheckpointEventsComponent::onPlayerLeaveCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerLeaveCheckpoint", { v8objPlayer });
    });
}

void CheckpointEventsComponent::onPlayerEnterRaceCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerEnterRaceCheckpoint", { v8objPlayer });
    });
}

void CheckpointEventsComponent::onPlayerLeaveRaceCheckpoint(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerLeaveRaceCheckpoint", { v8objPlayer });
    });
}

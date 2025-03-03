
#include "components/events/CustomModelEventsComponent.hpp"
#include "components/PlayerComponent.hpp"

#include "NodejsComponent.hpp"
#include "Utils.hpp"

CustomModelEventsComponent::CustomModelEventsComponent(ICore* core, ICustomModelsComponent* customModels, ResourceManager* resourceManager)
    : m_core(core)
    , m_customModels(customModels)
    , m_resourceManager(resourceManager)
{
    if (m_customModels)
    {
        m_customModels->getEventDispatcher().addEventHandler(this);
    }
}

CustomModelEventsComponent::~CustomModelEventsComponent()
{
}

void CustomModelEventsComponent::freeExtension()
{
    delete this;
}

void CustomModelEventsComponent::reset()
{
}

// =========== events
void CustomModelEventsComponent::onPlayerFinishedDownloading(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerFinishedDownloading", { v8objPlayer });
    });
}

bool CustomModelEventsComponent::onPlayerRequestDownload(IPlayer& player, ModelDownloadType type, uint32_t checksum)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        auto v8type      = v8::Integer::New(resource->m_isolate, (int)type);
        auto v8checksum  = v8::Integer::New(resource->m_isolate, (int)checksum);

        resource->Emit("onPlayerRequestDownload", { v8objPlayer, v8type, v8checksum });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

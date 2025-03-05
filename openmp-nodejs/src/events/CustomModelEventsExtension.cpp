
#include "events/CustomModelEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"

#include "NodejsComponent.hpp"
#include "Utils.hpp"

CustomModelEventsExtension::CustomModelEventsExtension(ICore* core, ICustomModelsComponent* customModels, ResourceManager* resourceManager)
    : m_core(core)
    , m_customModels(customModels)
    , m_resourceManager(resourceManager)
{
    if (m_customModels)
    {
        m_customModels->getEventDispatcher().addEventHandler(this);
    }
}

CustomModelEventsExtension::~CustomModelEventsExtension()
{
}

void CustomModelEventsExtension::freeExtension()
{
    delete this;
}

void CustomModelEventsExtension::reset()
{
}

// =========== events
void CustomModelEventsExtension::onPlayerFinishedDownloading(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerFinishedDownloading", { v8objPlayer });
    });
}

bool CustomModelEventsExtension::onPlayerRequestDownload(IPlayer& player, ModelDownloadType type, uint32_t checksum)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        auto v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        auto v8type      = v8::Integer::New(resource->m_isolate, (int)type);
        auto v8checksum  = v8::Integer::New(resource->m_isolate, (int)checksum);

        resource->Emit("onPlayerRequestDownload", { v8objPlayer, v8type, v8checksum });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

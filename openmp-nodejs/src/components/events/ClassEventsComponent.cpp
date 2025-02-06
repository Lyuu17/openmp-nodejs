
#include "components/events/ClassEventsComponent.hpp"
#include "components/PlayerComponent.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

ClassEventsComponent::ClassEventsComponent(ICore* core, IClassesComponent* classes, ResourceManager* resourceManager)
    : m_core(core)
    , m_classes(classes)
    , m_resourceManager(resourceManager)
{
    if (m_classes)
    {
        m_classes->getEventDispatcher().addEventHandler(this);
    }
}

ClassEventsComponent::~ClassEventsComponent()
{
    if (m_core)
    {
    }
}

void ClassEventsComponent::freeExtension()
{
    delete this;
}

void ClassEventsComponent::reset()
{
}

// =========== events

bool ClassEventsComponent::onPlayerRequestClass(IPlayer& player, unsigned int classId)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Integer> v8classId   = v8::Integer::New(resource->m_isolate, classId);

        resource->Emit("onPlayerRequestClass", { cancellableEventObj, v8objPlayer, v8classId });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

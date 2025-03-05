
#include "events/ClassEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"

#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

ClassEventsExtension::ClassEventsExtension(ICore* core, IClassesComponent* classes, ResourceManager* resourceManager)
    : m_core(core)
    , m_classes(classes)
    , m_resourceManager(resourceManager)
{
    if (m_classes)
    {
        m_classes->getEventDispatcher().addEventHandler(this);
    }
}

ClassEventsExtension::~ClassEventsExtension()
{
    if (m_core)
    {
    }
}

void ClassEventsExtension::freeExtension()
{
    delete this;
}

void ClassEventsExtension::reset()
{
}

// =========== events

bool ClassEventsExtension::onPlayerRequestClass(IPlayer& player, unsigned int classId)
{
    bool cancelled = false;

    m_resourceManager->Exec([&](Resource* resource) {
        auto cancellableEventObj = Utils::CancellableEventObject();

        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Integer> v8classId   = v8::Integer::New(resource->m_isolate, classId);

        resource->Emit("onPlayerRequestClass", { cancellableEventObj, v8objPlayer, v8classId });

        auto v8cancelledValue = cancellableEventObj->Get(resource->m_isolate->GetCurrentContext(), Utils::v8Str("cancelled"));
        cancelled             = !v8cancelledValue.IsEmpty() && v8cancelledValue.ToLocalChecked()->BooleanValue(resource->m_isolate);
    });

    return !cancelled;
}

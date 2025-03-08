
#include "events/PawnEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"

#include "NodejsComponent.hpp"
#include "Utils.hpp"

PawnEventsExtension::PawnEventsExtension(ICore* core, IPawnComponent* pawn, ResourceManager* resourceManager)
    : m_core(core)
    , m_pawn(pawn)
    , m_resourceManager(resourceManager)
{
    if (m_pawn)
    {
        m_pawn->getEventDispatcher().addEventHandler(this);
    }
}

PawnEventsExtension::~PawnEventsExtension()
{
}

void PawnEventsExtension::freeExtension()
{
    delete this;
}

void PawnEventsExtension::reset()
{
}

// =========== events
void PawnEventsExtension::onAmxLoad(IPawnScript& script)
{
    m_resourceManager->Exec([&](Resource* resource) {
        resource->Emit("onAmxLoad", {});
    });
}

void PawnEventsExtension::onAmxUnload(IPawnScript& script)
{
    m_resourceManager->Exec([&](Resource* resource) {
        resource->Emit("onAmxUnload", {});
    });
}

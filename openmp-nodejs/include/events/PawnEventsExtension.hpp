
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Pawn/pawn.hpp>
#include <sdk.hpp>

struct PawnEventsExtension final : public IExtension,
                                   public PawnEventHandler
{
    PROVIDE_EXT_UID(0x9249361CC82AC202);

    ICore*           m_core            = nullptr;
    IPawnComponent*  m_pawn            = nullptr;
    ResourceManager* m_resourceManager = nullptr;

public:
    PawnEventsExtension(ICore*, IPawnComponent*, ResourceManager*);
    ~PawnEventsExtension();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onAmxLoad(IPawnScript& script) override;
    void onAmxUnload(IPawnScript& script) override;
};

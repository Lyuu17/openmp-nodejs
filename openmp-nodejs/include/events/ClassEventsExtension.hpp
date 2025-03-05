
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Classes/classes.hpp>
#include <sdk.hpp>

struct ClassEventsExtension final : public IExtension,
                                    public ClassEventHandler
{
    PROVIDE_EXT_UID(0xE50A4B0A2D6CE2C0);

    ICore*             m_core            = nullptr;
    IClassesComponent* m_classes         = nullptr;
    ResourceManager*   m_resourceManager = nullptr;

public:
    ClassEventsExtension(ICore*, IClassesComponent* classes, ResourceManager* resourceManager);
    ~ClassEventsExtension();

    void freeExtension() override;
    void reset() override;

    /* events */
    bool onPlayerRequestClass(IPlayer& player, unsigned int classId) override;
};

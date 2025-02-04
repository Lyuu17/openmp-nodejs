
#pragma once

#include "common.hpp"

#include <Server/Components/Classes/classes.hpp>
#include <sdk.hpp>

struct ClassEventsComponent final : public IExtension,
                                    public ClassEventHandler
{
    PROVIDE_EXT_UID(0xE50A4B0A2D6CE2C0);

    ICore*             m_core    = nullptr;
    IClassesComponent* m_classes = nullptr;

public:
    ClassEventsComponent(ICore*, IClassesComponent* classes);
    ~ClassEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    bool onPlayerRequestClass(IPlayer& player, unsigned int classId) override;
};


#pragma once

#include "common.hpp"

#include "Resource.hpp"

#include <Server/Components/Databases/databases.hpp>
#include <sdk.hpp>

struct DbExtension final : public IExtension
{
    PROVIDE_EXT_UID(0x5B97BE641A408747);

    ICore*               m_core;
    IDatabasesComponent* m_databases;

    std::vector<IDatabaseConnection*> m_databaseConnections;

public:
    DbExtension(ICore* core, IDatabasesComponent* databases);
    ~DbExtension();

    void freeExtension() override;
    void reset() override;

    void InitFunctions(Resource*);
};


#pragma once

#include <Server/Components/Classes/classes.hpp>
#include <Server/Components/Databases/databases.hpp>
#include <Server/Components/Vehicles/vehicles.hpp>
#include <sdk.hpp>

#include "common.hpp"

#define PRINTLN(x, ...) \
    NodejsComponent::getInstance()->getCore()->printLn(std::format(x, __VA_ARGS__).c_str());

#define LOGLN(logLevel, x, ...) \
    NodejsComponent::getInstance()->getCore()->logLn(logLevel, std::format(x, __VA_ARGS__).c_str());

struct NodejsComponent : public IComponent,
                         public CoreEventHandler
{
    PROVIDE_UID(0xFBE82BE682AB46C8);

private:
    static NodejsComponent* m_instance;

    ICore* m_core = nullptr;

    IClassesComponent*   m_classes   = nullptr;
    IDatabasesComponent* m_databases = nullptr;
    IVehiclesComponent*  m_vehicles  = nullptr;

public:
    std::unique_ptr<node::MultiIsolatePlatform> m_platform;
    node::ArrayBufferAllocator*                 m_bufferAllocator = nullptr;

    ~NodejsComponent();

    StringView      componentName() const override;
    SemanticVersion componentVersion() const override;

    void onLoad(ICore* c) override;
    void onInit(IComponentList* components) override;
    void free() override;
    void reset() override;
    void onTick(std::chrono::microseconds elapsed, std::chrono::steady_clock::time_point now) override;

    ICore*               getCore();
    IClassesComponent*   getClasses();
    IDatabasesComponent* getDatabases();
    IVehiclesComponent*  getVehicles();

    static NodejsComponent* getInstance();
};

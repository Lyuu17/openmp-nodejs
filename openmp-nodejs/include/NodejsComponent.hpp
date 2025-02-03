
#pragma once

#include <sdk.hpp>

#include "common.hpp"

#define PRINTLN(x, ...) \
    NodejsComponent::getInstance()->getCore()->printLn(std::format(x, __VA_ARGS__).c_str());

#define LOGLN(logLevel, x, ...) \
    NodejsComponent::getInstance()->getCore()->logLn(logLevel, std::format(x, __VA_ARGS__).c_str());

struct NodejsComponent : public IComponent,
                         public CoreEventHandler,
                         public PlayerConnectEventHandler
{
    PROVIDE_UID(0xFBE82BE682AB46C8);

private:
    static NodejsComponent* m_instance;

    ICore* m_core = nullptr;

public:
    std::unique_ptr<node::MultiIsolatePlatform> m_platform;
    node::ArrayBufferAllocator*                 m_bufferAllocator = nullptr;

    ~NodejsComponent();

    StringView      componentName() const override;
    SemanticVersion componentVersion() const override;

    void onLoad(ICore* c) override;
    void free() override;
    void reset() override;
    void onTick(std::chrono::microseconds elapsed, std::chrono::steady_clock::time_point now) override;

    /* === player handlers === */
    void onIncomingConnection(IPlayer& player, StringView ipAddress, unsigned short port) override;
    void onPlayerConnect(IPlayer& player) override;
    void onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason) override;
    void onPlayerClientInit(IPlayer& player) override;

    ICore*                  getCore();
    static NodejsComponent* getInstance();
};

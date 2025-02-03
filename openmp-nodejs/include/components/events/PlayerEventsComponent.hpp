
#pragma once

#include "common.hpp"

#include <sdk.hpp>

struct PlayerEventsComponent final : public IExtension,
                                     public PlayerConnectEventHandler,
                                     public PlayerTextEventHandler
{
    PROVIDE_EXT_UID(0x7069E1695D3C87EB);

    ICore* m_core = nullptr;

public:
    PlayerEventsComponent(ICore*);
    ~PlayerEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onIncomingConnection(IPlayer& player, StringView ipAddress, unsigned short port) override;
    void onPlayerConnect(IPlayer& player) override;
    void onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason) override;
    void onPlayerClientInit(IPlayer& player) override;
    bool onPlayerText(IPlayer& player, StringView message) override;
    bool onPlayerCommandText(IPlayer& player, StringView message) override;
};

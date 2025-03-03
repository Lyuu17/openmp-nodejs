
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/CustomModels/custommodels.hpp>
#include <sdk.hpp>

struct CustomModelEventsComponent final : public IExtension,
                                          public PlayerModelsEventHandler
{
    PROVIDE_EXT_UID(0x716969695D3C87EB);

    ICore*                  m_core            = nullptr;
    ICustomModelsComponent* m_customModels    = nullptr;
    ResourceManager*        m_resourceManager = nullptr;

public:
    CustomModelEventsComponent(ICore*, ICustomModelsComponent*, ResourceManager*);
    ~CustomModelEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onPlayerFinishedDownloading(IPlayer& player) override;
    bool onPlayerRequestDownload(IPlayer& player, ModelDownloadType type, uint32_t checksum) override;
};

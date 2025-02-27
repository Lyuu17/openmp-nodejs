
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Objects/objects.hpp>
#include <sdk.hpp>

struct ObjectEventsComponent final : public IExtension,
                                     public ObjectEventHandler
{
    PROVIDE_EXT_UID(0x48872A29CFD4F6EA);

    ICore*             m_core            = nullptr;
    IObjectsComponent* m_objects         = nullptr;
    ResourceManager*   m_resourceManager = nullptr;

public:
    ObjectEventsComponent(ICore*, IObjectsComponent*, ResourceManager*);
    ~ObjectEventsComponent();

    void freeExtension() override;
    void reset() override;

    /* events */
    void onMoved(IObject& object) override;
    void onPlayerObjectMoved(IPlayer& player, IPlayerObject& object) override;
    void onObjectSelected(IPlayer& player, IObject& object, int model, Vector3 position) override;
    void onPlayerObjectSelected(IPlayer& player, IPlayerObject& object, int model, Vector3 position) override;
    void onObjectEdited(IPlayer& player, IObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation) override;
    void onPlayerObjectEdited(IPlayer& player, IPlayerObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation) override;
    void onPlayerAttachedObjectEdited(IPlayer& player, int index, bool saved, const ObjectAttachmentSlotData& data) override;
};

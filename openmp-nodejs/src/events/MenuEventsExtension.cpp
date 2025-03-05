
#include "events/MenuEventsExtension.hpp"
#include "classes/PlayerExtension.hpp"
#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

MenuEventsExtension::MenuEventsExtension(ICore* core, IMenusComponent* menus, ResourceManager* resourceManager)
    : m_core(core)
    , m_menus(menus)
    , m_resourceManager(resourceManager)
{
    if (menus)
    {
        menus->getEventDispatcher().addEventHandler(this);
    }
}

MenuEventsExtension::~MenuEventsExtension()
{
    if (m_core)
    {
    }
}

void MenuEventsExtension::freeExtension()
{
    delete this;
}

void MenuEventsExtension::reset()
{
}

// =========== events

void MenuEventsExtension::onPlayerSelectedMenuRow(IPlayer& player, MenuRow row)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));
        v8::Local<v8::Integer> v8menuRow   = v8::Integer::New(resource->m_isolate, row);

        resource->Emit("onPlayerSelectedMenuRow", { v8objPlayer, v8menuRow });
    });
}

void MenuEventsExtension::onPlayerExitedMenu(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerExtension>(player));

        resource->Emit("onPlayerExitedMenu", { v8objPlayer });
    });
}


#include "components/events/MenuEventsComponent.hpp"
#include "components/PlayerComponent.hpp"
#include "NodejsComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"

MenuEventsComponent::MenuEventsComponent(ICore* core, IMenusComponent* menus, ResourceManager* resourceManager)
    : m_core(core)
    , m_menus(menus)
    , m_resourceManager(resourceManager)
{
    if (menus)
    {
        menus->getEventDispatcher().addEventHandler(this);
    }
}

MenuEventsComponent::~MenuEventsComponent()
{
    if (m_core)
    {
    }
}

void MenuEventsComponent::freeExtension()
{
    delete this;
}

void MenuEventsComponent::reset()
{
}

// =========== events

void MenuEventsComponent::onPlayerSelectedMenuRow(IPlayer& player, MenuRow row)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Integer> v8menuRow   = v8::Integer::New(resource->m_isolate, row);

        resource->Emit("onPlayerSelectedMenuRow", { v8objPlayer, v8menuRow });
    });
}

void MenuEventsComponent::onPlayerExitedMenu(IPlayer& player)
{
    m_resourceManager->Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerExitedMenu", { v8objPlayer });
    });
}


#include <sdk.hpp>

#include "components/MenuComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_MENU_HAS_COMPONENT(menu, component)                                                           \
    if (!queryExtension<component>(menu))                                                                    \
    {                                                                                                        \
        menu->addExtension(new component(menu, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<component>(menu));                                                             \
    }

MenuComponent::MenuComponent(IMenu* menu, ResourceManager* resourceManager)
    : m_menu(menu)
    , m_resourceManager(resourceManager)
{
}

MenuComponent::~MenuComponent()
{
}

void MenuComponent::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void MenuComponent::reset()
{
}

// ====================== methods ======================

void MenuComponent::destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    NodejsComponent::getInstance()->getPickups()->release(menuComponent->m_menu->getID());
}

void MenuComponent::disable(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    menuComponent->m_menu->disable();
}

void MenuComponent::showForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    menuComponent->m_menu->showForPlayer(*player);
}

void MenuComponent::hideForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    menuComponent->m_menu->hideForPlayer(*player);
}

void MenuComponent::addCell(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    auto v8text = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
    if (v8text.IsEmpty())
        return;

    auto text = Utils::strV8(v8text.ToLocalChecked());

    auto colId = Utils::GetIntegerFromV8Value(info[1]);
    if (!colId.has_value())
        return;

    info.GetReturnValue().Set(menuComponent->m_menu->addCell(text, colId.value()));
}

void MenuComponent::setColumnHeader(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    auto v8header = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
    if (v8header.IsEmpty())
        return;

    auto header = Utils::strV8(v8header.ToLocalChecked());

    auto colId = Utils::GetIntegerFromV8Value(info[1]);
    if (!colId.has_value())
        return;

    menuComponent->m_menu->setColumnHeader(header, colId.value());
}

void MenuComponent::getCell(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    auto colId = Utils::GetIntegerFromV8Value(info[1]);
    if (!colId.has_value())
        return;

    auto rowId = Utils::GetIntegerFromV8Value(info[1]);
    if (!rowId.has_value())
        return;

    auto cell = menuComponent->m_menu->getCell(colId.value(), rowId.value());
    info.GetReturnValue().Set(Utils::v8Str(cell.data()));
}

// ====================== accessors ======================

void MenuComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), menuComponent->m_menu->getID()));
}

void MenuComponent::getEnabled(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), menuComponent->m_menu->isEnabled()));
}

void MenuComponent::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto menuComponent = (MenuComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<MenuComponent>(info.GetIsolate(), menuComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector2(menuComponent->m_menu->getPosition()));
}

v8::Local<v8::Object> MenuComponent::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("destroy", destroy);
    SET_FUNCTION("disable", disable);
    SET_FUNCTION("showForPlayer", showForPlayer);
    SET_FUNCTION("hideForPlayer", hideForPlayer);
    SET_FUNCTION("addCell", addCell);
    SET_FUNCTION("setColumnHeader", setColumnHeader);
    SET_FUNCTION("getCell", getCell);

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR("enabled", getEnabled);
    SET_ACCESSOR("position", getPosition);

    return v8obj;
}

void MenuComponent::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("getMenu"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8int = Utils::GetIntegerFromV8Value(info[0]);
        if (!v8int.has_value())
            return;

        auto menu = NodejsComponent::getInstance()->getMenus()->get(v8int.value());
        if (!menu)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        ENSURE_MENU_HAS_COMPONENT(menu, MenuComponent);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<MenuComponent>(menu)));
    }, v8::External::New(isolate, resource))
                                                                 .ToLocalChecked());

    context->Global()->Set(context, Utils::v8Str("createMenu"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8title = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
        if (v8title.IsEmpty())
            return;

        auto title = Utils::strV8(v8title.ToLocalChecked());

        auto v8position  = Utils::vector3V8(info[1]);
        auto v8columns   = Utils::GetIntegerFromV8Value(info[2]);
        auto v8col1Width = Utils::GetIntegerFromV8Value(info[3]);
        auto v8col2Width = Utils::GetIntegerFromV8Value(info[4]);

        if (!v8position.has_value() || !v8columns.has_value() || !v8col1Width.has_value() || !v8col2Width.has_value())
        {
            return;
        }

        auto menu = NodejsComponent::getInstance()->getMenus()->create(title, v8position.value(), v8columns.value(), v8col1Width.value(), v8col2Width.value());
        if (menu == nullptr)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        ENSURE_MENU_HAS_COMPONENT(menu, MenuComponent);

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<MenuComponent>(menu)));
    }, v8::External::New(isolate, resource))
                                                                    .ToLocalChecked());
}

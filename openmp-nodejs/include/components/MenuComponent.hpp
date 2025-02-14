
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"

#include <Server/Components/Menus/menus.hpp>
#include <sdk.hpp>

struct MenuComponent final : public IExtension,
                             public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0xA53572C5974AD6D1);

    IMenu*           m_menu;
    ResourceManager* m_resourceManager;

    MenuComponent(IMenu*, ResourceManager*);
    ~MenuComponent();

    void freeExtension() override;
    void reset() override;

    static void destroy(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void disable(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void showForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void hideForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void addCell(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setColumnHeader(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void getCell(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getEnabled(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    // static void setEnabled(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};

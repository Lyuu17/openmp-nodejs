
#pragma once

#include "common.hpp"

#include "ResourceManager.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <Server/Components/TextDraws/textdraws.hpp>
#include <sdk.hpp>

struct TextdrawComponent final : public IExtension,
                                 public IJavaScriptClassExtension
{
    PROVIDE_EXT_UID(0x88FB0AC44120E3E9);

    ITextDrawBase*   m_textdraw;
    IPlayer*         m_player;
    ResourceManager* m_resourceManager;

    TextdrawComponent(ITextDrawBase*, IPlayer*, ResourceManager*);
    ~TextdrawComponent();

    void freeExtension() override;
    void reset() override;

    static void destroy(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void showForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void isShownForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void setTextForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info);
    static void restream(const v8::FunctionCallbackInfo<v8::Value>& info);

    static void getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getText(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setText(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getLetterSize(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setLetterSize(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getTextSize(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setTextSize(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getAlignment(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setAlignment(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getBox(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setBox(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getBoxColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setBoxColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getShadow(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setShadow(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getOutline(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setOutline(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getBackgroundColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setBackgroundColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getStyle(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setStyle(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getProportional(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setProportional(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getSelectable(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setSelectable(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPreviewModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPreviewModel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPreviewRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPreviewRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPreviewVehicleColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPreviewVehicleColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getPreviewZoom(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setPreviewZoom(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static void getVisible(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void setVisible(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    v8::Local<v8::Object> CreateJavaScriptObject() override;

    static void InitFunctions(Resource*);
};

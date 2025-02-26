
#include <sdk.hpp>

#include "components/TextdrawComponent.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

#define ENSURE_TEXTDRAW_HAS_COMPONENT(textdraw, player, component)                                                           \
    if (!queryExtension<component>(textdraw))                                                                                \
    {                                                                                                                        \
        textdraw->addExtension(new component(textdraw, player, NodejsComponent::getInstance()->getResourceManager()), true); \
        assert(queryExtension<component>(textdraw));                                                                         \
    }

TextdrawComponent::TextdrawComponent(ITextDrawBase* textdraw, IPlayer* player, ResourceManager* resourceManager)
    : m_textdraw(textdraw)
    , m_player(player)
    , m_resourceManager(resourceManager)
{
}

TextdrawComponent::~TextdrawComponent()
{
}

void TextdrawComponent::freeExtension()
{
    m_resourceManager->Exec([this](Resource* resource) {
        resource->RemoveExtension(this);
    });

    delete this;
}

void TextdrawComponent::reset()
{
}

// ====================== methods ======================

void TextdrawComponent::destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto textdraw = dynamic_cast<ITextDraw*>(textdrawComponent->m_textdraw);
    if (textdraw)
    {
        NodejsComponent::getInstance()->getTextDraws()->release(textdrawComponent->m_textdraw->getID());
    }

    auto playerTextdraw = dynamic_cast<IPlayerTextDraw*>(textdrawComponent->m_textdraw);
    if (playerTextdraw)
    {
        auto playerTextDrawData = queryExtension<IPlayerTextDrawData>(textdrawComponent->m_player);
        if (!playerTextDrawData)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        playerTextDrawData->release(textdrawComponent->m_textdraw->getID());
    }
}

void TextdrawComponent::showForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto textdraw = dynamic_cast<ITextDraw*>(textdrawComponent->m_textdraw);
    if (!textdraw)
        return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    auto v8show = Utils::GetBooleanFromV8Value(info[1]);
    if (!v8show.has_value())
        return;

    if (v8show.value())
        textdraw->showForPlayer(*player);
    else
        textdraw->hideForPlayer(*player);
}

void TextdrawComponent::isShownForPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto textdraw = dynamic_cast<ITextDraw*>(textdrawComponent->m_textdraw);
    if (!textdraw)
        return;

    auto playerId = Utils::GetIdFromV8Object(info[0]);
    if (!playerId.has_value())
        return;

    auto player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
    if (!player)
        return;

    info.GetReturnValue().Set(textdraw->isShownForPlayer(*player));
}

void TextdrawComponent::restream(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    textdrawComponent->m_textdraw->restream();
}

// ====================== accessors ======================

void TextdrawComponent::getId(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();

    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), textdrawComponent->m_textdraw->getID()));
}

void TextdrawComponent::getPosition(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector2(textdrawComponent->m_textdraw->getPosition()));
}

void TextdrawComponent::setPosition(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto position = Utils::vector2V8(value);
    if (!position.has_value()) return;

    textdrawComponent->m_textdraw->setPosition(position.value());
}

void TextdrawComponent::getText(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Str(textdrawComponent->m_textdraw->getText().data()));
}

void TextdrawComponent::setText(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto text = Utils::strV8(value);
    if (!text.has_value()) return;

    textdrawComponent->m_textdraw->setText(text.value());
}

void TextdrawComponent::getLetterSize(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector2(textdrawComponent->m_textdraw->getLetterSize()));
}

void TextdrawComponent::setLetterSize(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto size = Utils::vector2V8(value);
    if (!size.has_value()) return;

    textdrawComponent->m_textdraw->setLetterSize(size.value());
}

void TextdrawComponent::getTextSize(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector2(textdrawComponent->m_textdraw->getLetterSize()));
}

void TextdrawComponent::setTextSize(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto size = Utils::vector2V8(value);
    if (!size.has_value()) return;

    textdrawComponent->m_textdraw->setTextSize(size.value());
}

void TextdrawComponent::getAlignment(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), textdrawComponent->m_textdraw->getAlignment()));
}

void TextdrawComponent::setAlignment(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setAlignment((TextDrawAlignmentTypes)v.value());
}

void TextdrawComponent::getColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Colour(textdrawComponent->m_textdraw->getLetterColour()));
}

void TextdrawComponent::setColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::colourV8(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setColour(v.value());
}

void TextdrawComponent::getBox(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->hasBox());
}

void TextdrawComponent::setBox(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetBooleanFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->useBox(v.value());
}

void TextdrawComponent::getBoxColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Colour(textdrawComponent->m_textdraw->getBoxColour()));
}

void TextdrawComponent::setBoxColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::colourV8(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setBoxColour(v.value());
}

void TextdrawComponent::getShadow(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->getShadow());
}

void TextdrawComponent::setShadow(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setShadow(v.value());
}

void TextdrawComponent::getOutline(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->getOutline());
}

void TextdrawComponent::setOutline(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setOutline(v.value());
}

void TextdrawComponent::getBackgroundColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Colour(textdrawComponent->m_textdraw->getBackgroundColour()));
}

void TextdrawComponent::setBackgroundColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::colourV8(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setBackgroundColour(v.value());
}

void TextdrawComponent::getStyle(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set((int)textdrawComponent->m_textdraw->getStyle());
}

void TextdrawComponent::setStyle(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setStyle((TextDrawStyle)v.value());
}

void TextdrawComponent::getProportional(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->isProportional());
}

void TextdrawComponent::setProportional(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetDoubleFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setProportional((TextDrawStyle)v.value());
}

void TextdrawComponent::getSelectable(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->isSelectable());
}

void TextdrawComponent::setSelectable(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetBooleanFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setSelectable(v.value());
}

void TextdrawComponent::getPreviewModel(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->getPreviewModel());
}

void TextdrawComponent::setPreviewModel(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetIntegerFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setPreviewModel(v.value());
}

void TextdrawComponent::getPreviewRotation(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(Utils::v8Vector3(textdrawComponent->m_textdraw->getPreviewRotation()));
}

void TextdrawComponent::setPreviewRotation(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::vector3V8(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setPreviewRotation(v.value());
}

void TextdrawComponent::getPreviewVehicleColour(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v8obj = v8::Array::New(info.GetIsolate(), 2);
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), 0, v8::Integer::New(info.GetIsolate(), textdrawComponent->m_textdraw->getPreviewVehicleColour().first));
    v8obj->Set(info.GetIsolate()->GetCurrentContext(), 1, v8::Integer::New(info.GetIsolate(), textdrawComponent->m_textdraw->getPreviewVehicleColour().second));

    info.GetReturnValue().Set(v8obj);
}

void TextdrawComponent::setPreviewVehicleColour(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    if (value.IsEmpty() || !value->IsArray())
        return;

    auto v8col1 = Utils::GetIntegerFromV8Value(value.As<v8::Object>()->Get(info.GetIsolate()->GetCurrentContext(), 0));
    auto v8col2 = Utils::GetIntegerFromV8Value(value.As<v8::Object>()->Get(info.GetIsolate()->GetCurrentContext(), 1));

    textdrawComponent->m_textdraw->setPreviewVehicleColour(v8col1.value_or(0), v8col2.value_or(0));
}

void TextdrawComponent::getPreviewZoom(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    info.GetReturnValue().Set(textdrawComponent->m_textdraw->getPreviewZoom());
}

void TextdrawComponent::setPreviewZoom(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto v = Utils::GetDoubleFromV8Value(value);
    if (!v.has_value()) return;

    textdrawComponent->m_textdraw->setPreviewZoom(v.value());
}

void TextdrawComponent::getVisible(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto textdraw = dynamic_cast<IPlayerTextDraw*>(textdrawComponent->m_textdraw);
    if (!textdraw)
        return;

    info.GetReturnValue().Set(textdraw->isShown());
}

void TextdrawComponent::setVisible(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    auto textdrawComponent = (TextdrawComponent*)info.Data().As<v8::External>()->Value();
    if (!Utils::CheckExtensionExist<TextdrawComponent>(info.GetIsolate(), textdrawComponent)) return;

    auto textdraw = dynamic_cast<IPlayerTextDraw*>(textdrawComponent->m_textdraw);
    if (!textdraw)
        return;

    auto v = Utils::GetBooleanFromV8Value(value);
    if (!v.has_value()) return;

    if (v.value())
        textdraw->show();
    else
        textdraw->hide();
}

v8::Local<v8::Object> TextdrawComponent::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(isolate, this)).ToLocalChecked());

    SET_FUNCTION("destroy", destroy);
    SET_FUNCTION("restream", restream);

    if (dynamic_cast<ITextDraw*>(m_textdraw))
    {
        SET_FUNCTION("showForPlayer", showForPlayer);
        SET_FUNCTION("isShownForPlayer", isShownForPlayer);
    }

#define SET_ACCESSOR(f, getter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, nullptr, v8::External::New(isolate, this));
#define SET_ACCESSOR_WITH_SETTER(f, getter, setter) v8obj->SetNativeDataProperty(context, Utils::v8Str(f), getter, setter, v8::External::New(isolate, this));

    SET_ACCESSOR("id", getId);
    SET_ACCESSOR_WITH_SETTER("position", getPosition, setPosition);
    SET_ACCESSOR_WITH_SETTER("text", getText, setText);
    SET_ACCESSOR_WITH_SETTER("letterSize", getLetterSize, setLetterSize);
    SET_ACCESSOR_WITH_SETTER("textSize", getTextSize, setTextSize);
    SET_ACCESSOR_WITH_SETTER("alignment", getAlignment, setAlignment);
    SET_ACCESSOR_WITH_SETTER("colour", getColour, setColour);
    SET_ACCESSOR_WITH_SETTER("box", getBox, setBox);
    SET_ACCESSOR_WITH_SETTER("boxColor", getBoxColour, setBoxColour);
    SET_ACCESSOR_WITH_SETTER("shadow", getShadow, setShadow);
    SET_ACCESSOR_WITH_SETTER("outline", getOutline, setOutline);
    SET_ACCESSOR_WITH_SETTER("backgroundColor", getBackgroundColour, setBackgroundColour);
    SET_ACCESSOR_WITH_SETTER("style", getStyle, setStyle);
    SET_ACCESSOR_WITH_SETTER("proportional", getProportional, setProportional);
    SET_ACCESSOR_WITH_SETTER("selectable", getSelectable, setSelectable);
    SET_ACCESSOR_WITH_SETTER("previewModel", getPreviewModel, setPreviewModel);
    SET_ACCESSOR_WITH_SETTER("previewRotation", getPreviewRotation, setPreviewRotation);
    SET_ACCESSOR_WITH_SETTER("previewVehicleColour", getPreviewVehicleColour, setPreviewVehicleColour);
    SET_ACCESSOR_WITH_SETTER("previewZoom", getPreviewZoom, setPreviewZoom);

    if (dynamic_cast<IPlayerTextDraw*>(m_textdraw))
    {
        SET_ACCESSOR_WITH_SETTER("visible", getVisible, setVisible);
    }

    return v8obj;
}

void TextdrawComponent::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("createTextDraw"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8pos = Utils::vector2V8(info[0]);
        auto v8str = Utils::strV8(info[1]);
        if (!v8pos.has_value() || !v8str.has_value())
            return;

        IPlayer*       player   = nullptr;
        ITextDrawBase* textdraw = nullptr;

        auto playerId = Utils::GetIdFromV8Object(info[2]);
        if (playerId.has_value())
        {
            player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
            if (!player)
                return;

            auto playerTextDrawData = queryExtension<IPlayerTextDrawData>(player);
            if (!playerTextDrawData)
            {
                info.GetReturnValue().SetUndefined();
                return;
            }

            textdraw = playerTextDrawData->create(v8pos.value(), v8str.value());

            ENSURE_TEXTDRAW_HAS_COMPONENT(textdraw, player, TextdrawComponent);
        }
        else
        {
            textdraw = NodejsComponent::getInstance()->getTextDraws()->create(v8pos.value(), v8str.value());

            ENSURE_TEXTDRAW_HAS_COMPONENT(textdraw, nullptr, TextdrawComponent);
        }

        if (!textdraw)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<TextdrawComponent>(textdraw)));
    }, v8::External::New(isolate, resource))
                                                                        .ToLocalChecked());

    context->Global()->Set(context, Utils::v8Str("getTextDraw"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8id = Utils::GetIntegerFromV8Value(info[0]);
        if (!v8id.has_value())
            return;

        IPlayer*       player   = nullptr;
        ITextDrawBase* textdraw = nullptr;

        auto playerId = Utils::GetIdFromV8Object(info[1]);
        if (playerId.has_value())
        {
            player = NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value());
            if (!player)
                return;

            auto playerTextDrawData = queryExtension<IPlayerTextDrawData>(player);
            if (!playerTextDrawData)
            {
                info.GetReturnValue().SetUndefined();
                return;
            }

            textdraw = playerTextDrawData->get(v8id.value());

            if (textdraw) ENSURE_TEXTDRAW_HAS_COMPONENT(textdraw, player, TextdrawComponent);
        }
        else
        {
            textdraw = NodejsComponent::getInstance()->getTextDraws()->get(v8id.value());

            if (textdraw) ENSURE_TEXTDRAW_HAS_COMPONENT(textdraw, nullptr, TextdrawComponent);
        }

        if (!textdraw)
        {
            info.GetReturnValue().SetUndefined();
            return;
        }

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        info.GetReturnValue().Set(resource->ObjectFromExtension(queryExtension<TextdrawComponent>(textdraw)));
    }, v8::External::New(isolate, resource))
                                                                     .ToLocalChecked());
}

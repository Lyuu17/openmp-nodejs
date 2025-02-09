
#include "Resource.hpp"

#include "CommonFunctions.hpp"
#include "NodejsComponent.hpp"
#include "Utils.hpp"

void CommonFunctions::Init(Resource* resource)
{
    resource->AddFunction("print", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8str = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
        if (v8str.IsEmpty())
            return;

        PRINTLN("{}", Utils::strV8(v8str.ToLocalChecked()));
    });

    resource->AddFunction("printError", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8str = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
        if (v8str.IsEmpty())
            return;

        LOGLN(Error, "{}", Utils::strV8(v8str.ToLocalChecked()));
    });

    resource->AddFunction("printWarning", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto v8str = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
        if (v8str.IsEmpty())
            return;

        LOGLN(Warning, "{}", Utils::strV8(v8str.ToLocalChecked()));
    });

    resource->AddFunction("on", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        auto v8str  = info[0]->ToString(isolate->GetCurrentContext());
        auto v8func = info[1].As<v8::Function>();
        if (v8str.IsEmpty() || v8func.IsEmpty())
            return;

        resource->AddListener(Utils::strV8(v8str.ToLocalChecked()), v8func);
    }, resource);

    resource->AddFunction("emit", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        auto resource = (Resource*)info.Data().As<v8::External>()->Value();

        auto v8str = info[0]->ToString(isolate->GetCurrentContext());
        if (v8str.IsEmpty())
            return;

        std::vector<v8::Local<v8::Value>> args;
        for (int i = 1; i < info.Length(); i++)
            args.push_back(info[i]);

        std::initializer_list<v8::Local<v8::Value>> argsList;
        if (args.size() > 0)
            argsList = std::initializer_list<v8::Local<v8::Value>>(&(args.front()), &(args.back()));

        resource->Emit(Utils::strV8(v8str.ToLocalChecked()), argsList);
    }, resource);

    resource->AddFunction("message", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        auto v8messageStr = info[0]->ToString(isolate->GetCurrentContext());
        if (v8messageStr.IsEmpty())
            return;

        auto messageColor = Utils::GetIntegerFromV8Value(info[1]);

        Colour color = Colour::FromRGBA(messageColor.value_or(0xFFFFFFFF));

        NodejsComponent::getInstance()->getCore()->getPlayers().sendClientMessageToAll(color, Utils::strV8(v8messageStr.ToLocalChecked()).c_str());
    });

    resource->AddFunction("messagePlayer", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        auto v8messageStr = info[0]->ToString(isolate->GetCurrentContext());
        if (v8messageStr.IsEmpty())
            return;

        auto v8player = info[1]->ToObject(isolate->GetCurrentContext());
        if (v8player.IsEmpty())
            return;

        auto playerId = Utils::GetIdFromV8Object(v8player);
        if (!playerId.has_value())
            return;

        auto messageColor = Utils::GetIntegerFromV8Value(info[2]);

        Colour color = Colour::FromRGBA(messageColor.value_or(0xFFFFFFFF));

        NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value())->sendClientMessage(color, Utils::strV8(v8messageStr.ToLocalChecked()).c_str());
    });

    resource->AddFunction("Vector2", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        if (!info.IsConstructCall())
            return;

        auto v8x = Utils::GetDoubleFromV8Value(info[0]);
        auto v8y = Utils::GetDoubleFromV8Value(info[1]);

        Vector2 vec2 { v8x.value_or(0.0), v8y.value_or(0.0) };

        info.GetReturnValue().Set(Utils::v8Vector2(vec2));
    });

    resource->AddFunction("Vector3", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        if (!info.IsConstructCall())
            return;

        auto v8x = Utils::GetDoubleFromV8Value(info[0]);
        auto v8y = Utils::GetDoubleFromV8Value(info[1]);
        auto v8z = Utils::GetDoubleFromV8Value(info[2]);

        Vector3 vec3 { v8x.value_or(0.0), v8y.value_or(0.0), v8z.value_or(0.0) };

        info.GetReturnValue().Set(Utils::v8Vector3(vec3));
    });
}

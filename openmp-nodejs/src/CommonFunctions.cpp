
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

        resource->Emit(Utils::strV8(v8str.ToLocalChecked()), args);
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

        auto playerId = Utils::GetIdFromV8Object(info[1]);
        if (!playerId.has_value())
            return;

        auto messageColor = Utils::GetIntegerFromV8Value(info[2]);

        Colour color = Colour::FromRGBA(messageColor.value_or(0xFFFFFFFF));

        NodejsComponent::getInstance()->getCore()->getPlayers().get(playerId.value())->sendClientMessage(color, Utils::strV8(v8messageStr.ToLocalChecked()).c_str());
    });

    resource->AddFunction("Quat", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto isolate = info.GetIsolate();

        if (!info.IsConstructCall())
            return;

        auto v8w = Utils::GetDoubleFromV8Value(info[0]);
        auto v8x = Utils::GetDoubleFromV8Value(info[1]);
        auto v8y = Utils::GetDoubleFromV8Value(info[2]);
        auto v8z = Utils::GetDoubleFromV8Value(info[3]);

        auto v8vec3 = Utils::vector3V8(info[0]);

        GTAQuat quat {};
        if (v8w.has_value() && v8x.has_value() && v8y.has_value() && v8z.has_value())
            quat = GTAQuat { (float)v8w.value(), (float)v8x.value(), (float)v8y.value(), (float)v8z.value() };
        else if (v8vec3.has_value())
            quat = GTAQuat { v8vec3.value() };

        info.GetReturnValue().Set(Utils::v8Quat(quat));
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

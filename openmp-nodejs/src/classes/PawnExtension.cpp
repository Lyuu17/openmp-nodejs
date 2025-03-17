
#include <sdk.hpp>

#include "classes/PawnExtension.hpp"

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

// ====================== methods ======================

void PawnExtension::callPublicFunction(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto pawnComponent = NodejsComponent::getInstance()->getPawn();
    if (!pawnComponent) return;

    auto name = Utils::strV8(info[0]);
    auto args = Utils::strV8(info[1]);

    if (!args.has_value()) args = "";

    if (!name.has_value()) return;

    auto mainScript = pawnComponent->mainScript();
    if (mainScript)
    {
        int  idx {};
        cell ret = DefaultReturnValue_False;
        if (!mainScript->FindPublic(name.value().c_str(), &idx))
        {
            Utils::v8PawnCall(idx, ret, mainScript, args.value(), info);

            info.GetReturnValue().Set(ret);
            return;
        }
    }

    auto sideScripts = pawnComponent->sideScripts();
    for (auto sideScript : sideScripts)
    {
        int  idx {};
        cell ret = DefaultReturnValue_False;
        if (!sideScript->FindPublic(name.value().c_str(), &idx))
        {
            Utils::v8PawnCall(idx, ret, sideScript, args.value(), info);

            info.GetReturnValue().Set(ret);
            return;
        }
    }
}

void PawnExtension::callNativeFunction(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto pawnComponent = NodejsComponent::getInstance()->getPawn();
    if (!pawnComponent) return;

    auto name = Utils::strV8(info[0]);
    auto args = Utils::strV8(info[1]);

    if (!args.has_value()) args = "";

    if (!name.has_value()) return;

    auto mainScript = pawnComponent->mainScript();
    if (mainScript)
    {
        int idx {};

        cell ret = DefaultReturnValue_False;
        if (!mainScript->FindNative(name.value().c_str(), &idx))
        {
            Utils::v8PawnCall(idx, ret, mainScript, args.value(), info);

            info.GetReturnValue().Set(ret);
            return;
        }
    }

    auto sideScripts = pawnComponent->sideScripts();
    for (auto sideScript : sideScripts)
    {
        int  idx {};
        cell ret = DefaultReturnValue_False;
        if (!sideScript->FindNative(name.value().c_str(), &idx))
        {
            Utils::v8PawnCall(idx, ret, sideScript, args.value(), info);

            info.GetReturnValue().Set(ret);
            return;
        }
    }
}

v8::Local<v8::Object> PawnExtension::CreateJavaScriptObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = v8::Object::New(isolate);

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func).ToLocalChecked());

    SET_FUNCTION("callPublicFunction", callPublicFunction);
    SET_FUNCTION("callNativeFunction", callNativeFunction);

    return v8obj;
}

void PawnExtension::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto v8obj = CreateJavaScriptObject();

    context->Global()->Set(context, Utils::v8Str("pawn"), v8obj);
}

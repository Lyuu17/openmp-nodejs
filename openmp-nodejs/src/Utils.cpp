
#include "Utils.hpp"
#include "NodejsComponent.hpp"

v8::Local<v8::String> Utils::v8Str(const std::string& x)
{
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x.c_str()).ToLocalChecked();
}

std::optional<std::string> Utils::strV8(v8::MaybeLocal<v8::Value> str)
{
    if (str.IsEmpty() || !str.ToLocalChecked()->IsString())
        return std::nullopt;
    v8::String::Utf8Value v8str { v8::Isolate::GetCurrent(), str.ToLocalChecked() };
    if (!*v8str)
        return std::nullopt;
    return { *v8str };
}

v8::Local<v8::Object> Utils::v8Vector2(Vector2 vec2)
{
    auto v8obj = v8::Object::New(v8::Isolate::GetCurrent());
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), vec2.x));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), vec2.y));
    return v8obj;
}

std::optional<Vector2> Utils::vector2V8(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsObject())
        return std::nullopt;

    auto vec3Obj = val.ToLocalChecked().As<v8::Object>();

    auto v8maybeX = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("x"));
    auto v8maybeY = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("y"));

    if (v8maybeX.IsEmpty() || v8maybeY.IsEmpty())
        return std::nullopt;

    Vector2 vec2 {
        GetDoubleFromV8Value(v8maybeX).value_or(0),
        GetDoubleFromV8Value(v8maybeY).value_or(0)
    };
    return vec2;
}

v8::Local<v8::Object> Utils::v8Vector3(Vector3 vec3)
{
    auto v8obj = v8::Object::New(v8::Isolate::GetCurrent());
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), vec3.x));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), vec3.y));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("z"), v8::Number::New(v8::Isolate::GetCurrent(), vec3.z));
    return v8obj;
}

std::optional<Vector3> Utils::vector3V8(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsObject())
        return std::nullopt;

    auto vec3Obj = val.ToLocalChecked().As<v8::Object>();

    auto v8maybeX = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("x"));
    auto v8maybeY = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("y"));
    auto v8maybeZ = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("z"));

    if (v8maybeX.IsEmpty() || v8maybeY.IsEmpty() || v8maybeZ.IsEmpty())
        return std::nullopt;

    Vector3 vec3 {
        GetDoubleFromV8Value(v8maybeX).value_or(0),
        GetDoubleFromV8Value(v8maybeY).value_or(0),
        GetDoubleFromV8Value(v8maybeZ).value_or(0)
    };
    return vec3;
}

v8::Local<v8::Object> Utils::v8Colour(const Colour& col)
{
    auto v8obj = v8::Object::New(v8::Isolate::GetCurrent());
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("r"), v8::Integer::New(v8::Isolate::GetCurrent(), col.r));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("g"), v8::Integer::New(v8::Isolate::GetCurrent(), col.g));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("b"), v8::Integer::New(v8::Isolate::GetCurrent(), col.b));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("a"), v8::Integer::New(v8::Isolate::GetCurrent(), col.a));
    return v8obj;
}

std::optional<Colour> Utils::colourV8(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsObject())
        return std::nullopt;

    auto colorObj = val.ToLocalChecked().As<v8::Object>();

    auto v8maybeR = colorObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("r"));
    auto v8maybeG = colorObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("g"));
    auto v8maybeB = colorObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("b"));
    auto v8maybeA = colorObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("a"));

    if (v8maybeR.IsEmpty() || v8maybeG.IsEmpty() || v8maybeB.IsEmpty())
        return std::nullopt;

    Colour color {
        (uint8_t)GetIntegerFromV8Value(v8maybeR).value_or(0),
        (uint8_t)GetIntegerFromV8Value(v8maybeG).value_or(0),
        (uint8_t)GetIntegerFromV8Value(v8maybeB).value_or(0),
        (uint8_t)GetIntegerFromV8Value(v8maybeA).value_or(0)
    };
    return color;
}

std::optional<GTAQuat> Utils::quatV8(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsObject())
        return std::nullopt;

    auto quatObj  = val.ToLocalChecked().As<v8::Object>();
    auto v8maybeX = quatObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("x"));
    auto v8maybeY = quatObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("y"));
    auto v8maybeZ = quatObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("z"));
    auto v8maybeW = quatObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("w"));
    if (v8maybeX.IsEmpty() || v8maybeY.IsEmpty() || v8maybeZ.IsEmpty())
        return std::nullopt;
    GTAQuat quat {
        (float)GetDoubleFromV8Value(v8maybeW).value_or(0),
        (float)GetDoubleFromV8Value(v8maybeX).value_or(0),
        (float)GetDoubleFromV8Value(v8maybeY).value_or(0),
        (float)GetDoubleFromV8Value(v8maybeZ).value_or(0)
    };
    return quat;
}

v8::Local<v8::Object> Utils::v8Quat(GTAQuat quat)
{
    auto v8obj = v8::Object::New(v8::Isolate::GetCurrent());
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), quat.q.x));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), quat.q.y));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("z"), v8::Number::New(v8::Isolate::GetCurrent(), quat.q.z));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("w"), v8::Number::New(v8::Isolate::GetCurrent(), quat.q.w));

    auto toEuler = v8::Function::New(v8::Isolate::GetCurrent()->GetCurrentContext(), [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto quat = Utils::quatV8(info.This());
        if (!quat.has_value())
        {
            return;
        }

        auto euler   = quat.value().ToEuler();
        auto v8euler = Utils::v8Vector3(euler);
        info.GetReturnValue().Set(v8euler);
    });

    if (!toEuler.IsEmpty())
        v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("toEuler"), toEuler.ToLocalChecked());

    return v8obj;
}

v8::Local<v8::Object> Utils::v8PlayerBulletData(const PlayerBulletData& playerBulletData)
{
    auto v8obj = v8::Object::New(v8::Isolate::GetCurrent());
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("origin"), v8Vector3(playerBulletData.origin));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("bodyPart"), v8Vector3(playerBulletData.hitPos));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("hitEntity"), v8Vector3(playerBulletData.offset));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("weapon"), v8::Integer::New(v8::Isolate::GetCurrent(), playerBulletData.weapon));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("hitType"), v8::Integer::New(v8::Isolate::GetCurrent(), playerBulletData.hitType));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("hitId"), v8::Integer::New(v8::Isolate::GetCurrent(), playerBulletData.hitID));
    return v8obj;
}

v8::Local<v8::Object> Utils::CancellableEventObject()
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    auto preventDefaultCb = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto context = info.GetIsolate()->GetCurrentContext();
        info.This()->DefineOwnProperty(context, v8Str("cancelled"), v8::Boolean::New(info.GetIsolate(), true), v8::PropertyAttribute::ReadOnly);

        info.GetReturnValue().Set(true);
    };

    auto v8obj = v8::Object::New(isolate);
    v8obj->DefineOwnProperty(context, v8Str("cancelled"), v8::Boolean::New(isolate, false), v8::PropertyAttribute::ReadOnly);

    v8obj->Set(context, Utils::v8Str("preventDefault"), v8::Function::New(context, preventDefaultCb).ToLocalChecked());

    return v8obj;
}

std::optional<int32_t> Utils::GetIntegerFromV8Value(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    if (val.ToLocalChecked()->IsNullOrUndefined())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsInt32())
        return std::nullopt;

    auto v8int = val.ToLocalChecked()->ToInteger(context);
    if (v8int.IsEmpty())
        return std::nullopt;

    auto mayv8int = v8int.ToLocalChecked()->Int32Value(context);
    if (mayv8int.IsNothing())
        return std::nullopt;
    return mayv8int.ToChecked();
}

std::optional<double> Utils::GetDoubleFromV8Value(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    if (val.ToLocalChecked()->IsNullOrUndefined())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsNumber())
        return std::nullopt;

    auto v8num = val.ToLocalChecked()->ToNumber(context);
    if (v8num.IsEmpty())
        return std::nullopt;

    return v8num.ToLocalChecked()->Value();
}

std::optional<bool> Utils::GetBooleanFromV8Value(v8::MaybeLocal<v8::Value> val)
{
    if (val.IsEmpty())
        return std::nullopt;

    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    if (val.ToLocalChecked()->IsNullOrUndefined())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsBoolean())
        return std::nullopt;

    auto v8num = val.ToLocalChecked()->ToBoolean(isolate);
    if (v8num.IsEmpty())
        return std::nullopt;

    return v8num->Value();
}

std::optional<uint32_t> Utils::GetIdFromV8Object(v8::MaybeLocal<v8::Value> val)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    if (val.IsEmpty())
        return std::nullopt;

    if (val.ToLocalChecked()->IsNullOrUndefined())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsObject())
        return std::nullopt;

    auto idField = val.ToLocalChecked().As<v8::Object>()->Get(context, Utils::v8Str("id"));
    if (idField.IsEmpty())
        return std::nullopt;

    auto id = idField.ToLocalChecked()->IntegerValue(context);
    if (id.IsNothing())
        return std::nullopt;

    return id.ToChecked();
}

std::optional<WeaponSlots> Utils::GetWeaponSlotsDataFromV8Object(v8::MaybeLocal<v8::Value> val)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    if (val.IsEmpty())
        return std::nullopt;

    if (val.ToLocalChecked()->IsNullOrUndefined())
        return std::nullopt;

    if (!val.ToLocalChecked()->IsArray())
        return std::nullopt;

    WeaponSlots weaponSlots;

    auto v8weaponsArray = val.ToLocalChecked().As<v8::Array>();
    for (int i = 0; i < v8weaponsArray->Length(); i++)
    {
        auto arraySlot = v8weaponsArray->Get(context, i);
        if (arraySlot.IsEmpty())
            continue;

        if (!arraySlot.ToLocalChecked()->IsObject())
            continue;

        auto wepId = Utils::GetIntegerFromV8Value(arraySlot.ToLocalChecked().As<v8::Object>()->Get(context, Utils::v8Str("id")));
        if (!wepId.has_value())
            continue;

        auto ammo = Utils::GetIntegerFromV8Value(arraySlot.ToLocalChecked().As<v8::Object>()->Get(context, Utils::v8Str("ammo")));
        if (!ammo.has_value())
            continue;

        WeaponSlotData weaponSlotData {
            (uint8_t)wepId.value(),
            (uint32_t)ammo.value()
        };

        weaponSlots[weaponSlotData.slot()] = weaponSlotData;
    }

    return weaponSlots;
}

void Utils::v8PawnCall(int idx, cell& ret, IPawnScript* script, const std::string& args, const v8::FunctionCallbackInfo<v8::Value>& info)
{
    cell amx_addr = script->GetHEA();

    int err {};
    for (int i = 2, arg = 0; i < info.Length() && arg < args.size(); i++, arg++)
    {
        if (info[i]->IsString())
        {
            err = script->PushString(nullptr, nullptr, Utils::strV8(info[i]).value(), false, false);
        }
        else if (info[i]->IsBoolean())
        {
            err = script->PushOne(Utils::GetBooleanFromV8Value(info[i]).value());
        }
        else if (args[arg] == 'i')
        {
            err = script->PushOne(Utils::GetIntegerFromV8Value(info[i]).value());
        }
        else if (args[arg] == 'f' || args[arg] == 'd')
        {
            err = script->PushOne(Utils::GetDoubleFromV8Value(info[i]).value());
        }

        if (err != AMX_ERR_NONE)
        {
            break;
        }
    }

    if (err == AMX_ERR_NONE)
        err = script->Exec(&ret, idx);

    if (err != AMX_ERR_NONE)
        script->PrintError(err);

    // Release everything at once.  Technically all that `Release` does is reset the heap back
    // to where it was before the call to any memory-allocating functions.  You could do that
    // for every allocating parameter in reverse order, or just do it once for all of them together.
    script->Release(amx_addr);
}

void Utils::PrintWavyUnderline(int start, int length)
{
    LOGLN(LogLevel::Error, "{}{}", std::string(start, ' ').c_str(), std::string(length, '^').c_str());
}

int Utils::CountLeadingSpaces(const char* str, int length)
{
    for (int i = 0; i < length; ++i)
    {
        if (str[i] != ' ')
        {
            return i;
        }
    }
    return length;
}

void Utils::PrintSourceLineWithUnderline(v8::Isolate* isolate, v8::Local<v8::Message> message, v8::Local<v8::Context> context)
{
    auto maybeSourceLine = message->GetSourceLine(context);
    if (maybeSourceLine.IsEmpty())
    {
        LOGLN(LogLevel::Error, "No source line available");
        return;
    }

    auto sourceLine = maybeSourceLine.ToLocalChecked();

    int length = sourceLine->Length();
    if (length > 256)
    {
        return;
    }

    v8::String::Utf8Value utf8SourceLine(isolate, sourceLine);
    const char*           sourceLineStr = *utf8SourceLine;

    LOGLN(LogLevel::Error, "{}", sourceLineStr);

    auto startCol = message->GetStartColumn(context);
    auto endCol   = message->GetEndColumn(context);

    if (startCol.IsNothing() || endCol.IsNothing())
    {
        return;
    }

    int start = startCol.FromJust();
    int end   = endCol.FromJust();

    if (start <= 0 || start >= length || end >= length)
    {
        return;
    }

    int leadingSpaces = CountLeadingSpaces(sourceLineStr, length);
    PrintWavyUnderline(start + leadingSpaces, end - start);
}

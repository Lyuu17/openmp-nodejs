
#include "Utils.hpp"
#include "NodejsComponent.hpp"

v8::Local<v8::String> Utils::v8Str(const std::string& x)
{
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x.c_str()).ToLocalChecked();
}

std::string Utils::strV8(v8::Local<v8::String> str)
{
    v8::String::Utf8Value v8str { v8::Isolate::GetCurrent(), str };
    return { *v8str };
}

v8::Local<v8::Object> Utils::v8Vector3(Vector3 vec3)
{
    auto v8obj = v8::Object::New(v8::Isolate::GetCurrent());
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), vec3.x));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), vec3.y));
    v8obj->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), v8Str("z"), v8::Number::New(v8::Isolate::GetCurrent(), vec3.z));
    return v8obj;
}

Vector3 Utils::vector3V8(v8::Local<v8::Object> vec3Obj)
{
    Vector3 vec3 {};

    auto v8maybeX = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("x"));
    if (!v8maybeX.IsEmpty())
        vec3.x = v8maybeX.ToLocalChecked()->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();

    auto v8maybeY = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("y"));
    if (!v8maybeY.IsEmpty())
        vec3.y = v8maybeY.ToLocalChecked()->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();

    auto v8maybeZ = vec3Obj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), Utils::v8Str("z"));
    if (!v8maybeZ.IsEmpty())
        vec3.z = v8maybeZ.ToLocalChecked()->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();
    return vec3;
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

    auto                  sourceLine = maybeSourceLine.ToLocalChecked();
    v8::String::Utf8Value utf8SourceLine(isolate, sourceLine);
    const char*           sourceLineStr = *utf8SourceLine;

    LOGLN(LogLevel::Error, "{}", sourceLineStr);

    auto startCol = message->GetStartColumn(context);
    auto endCol   = message->GetEndColumn(context);

    if (startCol.IsNothing() || endCol.IsNothing())
    {
        return;
    }

    int start  = startCol.FromJust();
    int end    = endCol.FromJust();
    int length = sourceLine->Length();

    if (start <= 0 || start >= length || end >= length)
    {
        return;
    }

    int leadingSpaces = CountLeadingSpaces(sourceLineStr, length);
    PrintWavyUnderline(start + leadingSpaces, end - start);
}

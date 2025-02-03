
#pragma once

namespace Utils
{
    v8::Local<v8::String> v8Str(const std::string& x);
    std::string           strV8(v8::Local<v8::String>);

    void PrintWavyUnderline(int start, int length);
    int  CountLeadingSpaces(const char* str, int length);
    void PrintSourceLineWithUnderline(v8::Isolate* isolate, v8::Local<v8::Message> message, v8::Local<v8::Context> context);
} // namespace Utils

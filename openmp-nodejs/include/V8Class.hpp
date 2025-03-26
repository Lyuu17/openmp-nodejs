
#pragma once

#include "common.hpp"
#include "Utils.hpp"

template <typename T>
class V8Class : std::unique_ptr<V8Class<T>> {

    v8::Local<v8::Object> m_obj;

public:
    V8Class() = delete;
    V8Class(v8::Local<v8::Object> obj)
        : m_obj(obj)
    {
    }

    v8::Local<v8::Object> operator->()
    {
        return m_obj;
    }

    inline v8::Local<v8::Object> get()
    {
        return m_obj;
    }

    static V8Class<T> NewClass(const std::string& name)
    {
        auto isolate = v8::Isolate::GetCurrent();

        V8Class v8class { v8::Object::New(isolate) };

        auto toStringTag = v8::Symbol::GetToStringTag(isolate);

        v8class->Set(isolate->GetCurrentContext(), toStringTag, v8::String::NewFromUtf8(isolate, name.c_str()).ToLocalChecked());

        return v8class;
    }
};


#pragma once

#include "common.hpp"
#include <sdk.hpp>

class Resource;

struct IJavaScriptClassExtension
{
    virtual v8::Local<v8::Object> CreateJavaScriptObject()
    {
        return {};
    }
};

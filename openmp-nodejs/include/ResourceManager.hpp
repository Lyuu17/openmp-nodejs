
#pragma once

#include "common.hpp"

#include "Resource.hpp"

class ResourceManager {
public:
    inline static std::unordered_map<std::filesystem::path, std::shared_ptr<Resource>> m_resources;

    static void Deinit();

    static void                       LoadResourcesFromPath(const std::filesystem::path& path);
    static void                       LoadResource(const std::filesystem::path& path);
    static void                       Exec(std::function<void(Resource* resource)> func);
    static Resource*                  GetResourceFromIsolate(v8::Isolate* isolate);
    static v8::MaybeLocal<v8::Module> ResolveModule(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer);
};

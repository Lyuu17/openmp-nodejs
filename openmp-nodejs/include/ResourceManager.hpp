
#pragma once

#include "common.hpp"

#include "Resource.hpp"

class ResourceManager : public IExtension {
public:
    PROVIDE_EXT_UID(0xB80687827766FA13);

    v8::Isolate*            m_isolate = nullptr;
    v8::Global<v8::Context> m_context;
    node::Environment*      m_parentEnv = nullptr;

    std::unique_ptr<node::MultiIsolatePlatform> m_platform;

    std::unordered_map<std::filesystem::path, ResourcePtr> m_resources;

    ResourceManager();
    ~ResourceManager();

    void freeExtension() override;
    void reset() override;

    void      Tick();
    void      LoadResourcesFromPath(const std::filesystem::path& path);
    void      LoadResource(const std::filesystem::path& path);
    void      Exec(std::function<void(Resource* resource)> func);
    Resource* GetResourceFromIsolate(v8::Isolate* isolate);
};

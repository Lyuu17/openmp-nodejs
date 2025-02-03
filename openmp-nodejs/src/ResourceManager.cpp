#include "ResourceManager.hpp"

#include "NodejsComponent.hpp"

void ResourceManager::Deinit()
{
    m_resources.clear();
}

void ResourceManager::LoadResourcesFromPath(const std::filesystem::path& path)
{
    std::filesystem::create_directories(path);

    for (auto const& dir_entry : std::filesystem::directory_iterator { path })
        LoadResource(dir_entry);
}

void ResourceManager::LoadResource(const std::filesystem::path& path)
{
    std::ifstream packageJson { path / "package.json" };
    if (!packageJson.is_open())
    {
        LOGLN(LogLevel::Error, "Failed to load resource folder {}, missing package.json", path.string());
        return;
    }

    std::string packageJsonContent((std::istreambuf_iterator<char>(packageJson)), std::istreambuf_iterator<char>());

    const auto folderName = path.filename().string();

    auto resource = std::make_shared<Resource>(path, folderName, packageJsonContent);

    auto [insertPair, result] = m_resources.emplace(path, std::move(resource));

    insertPair->second->InstantiateModule();

    PRINTLN("Found resource: {}", folderName);
}

void ResourceManager::Exec(std::function<void(Resource* resource)> func)
{
    for (auto const& [path, resource] : m_resources)
        resource->Exec(func);
}

Resource* ResourceManager::GetResourceFromIsolate(v8::Isolate* isolate)
{
    for (auto const& [path, resource] : m_resources)
    {
        if (resource->m_isolate == isolate)
            return resource.get();
    }
    return nullptr;
}

v8::MaybeLocal<v8::Module> ResourceManager::ResolveModule(v8::Local<v8::Context> context, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer)
{
    auto isolate = context->GetIsolate();

    auto resource = GetResourceFromIsolate(isolate);
    assert(resource);

    std::string importPath { *v8::String::Utf8Value(isolate, specifier) };

    auto maybeModule = resource->ResolveFile(importPath, referrer);
    if (maybeModule.IsEmpty())
    {
        resource->ThrowException(std::format("Failed to resolve module {}", importPath));
        return {};
    }

    return maybeModule;
}

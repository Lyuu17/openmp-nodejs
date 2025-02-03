
#pragma once

#include "common.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

class Resource;

using ResourcePtr = std::shared_ptr<Resource>;

class Resource {
public:
    const std::filesystem::path m_folderPath;
    const std::string           m_folderName;

    v8::Isolate* m_isolate;

    v8::Global<v8::Context> m_context;

    std::unordered_map<std::filesystem::path, v8::Global<v8::Module>>      m_modules;
    std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>> m_listeners;

    std::unordered_map<IJavaScriptClassExtension*, v8::Global<v8::Object>> m_objectsFromExtension;

    Resource(const std::filesystem::path& folderPath, const std::string& folderName, const std::string& packageJsonBuf);
    ~Resource();

    Resource(Resource&)            = delete;
    Resource& operator=(Resource&) = delete;

    void                       AddListener(const std::string& name, v8::Local<v8::Function> listener);
    void                       Emit(const std::string& name, std::initializer_list<v8::Local<v8::Value>> values);
    void                       AddFunction(const std::string& name, v8::FunctionCallback cb, void* userdata = nullptr);
    void                       ReportException(v8::TryCatch* try_catch);
    void                       InstantiateModule();
    v8::MaybeLocal<v8::Module> ResolveFile(const std::string& importPath, v8::Local<v8::Module> referrer);
    void                       ThrowException(const std::string& text);
    void                       Exec(std::function<void(Resource* resource)> func);

    inline v8::Local<v8::Object> ObjectFromExtension(IJavaScriptClassExtension* ext)
    {
        v8::EscapableHandleScope handle_scope(m_isolate);

        auto               context = m_isolate->GetCurrentContext();
        v8::Context::Scope context_scope(context);

        if (m_objectsFromExtension.contains(ext))
            return handle_scope.Escape(m_objectsFromExtension.at(ext).Get(m_isolate));

        auto v8obj = ext->CreateJavaScriptObject(this);

        m_objectsFromExtension[ext].Reset(m_isolate, v8obj);

        return handle_scope.Escape(m_objectsFromExtension.at(ext).Get(m_isolate));
    }

    void RemoveExtension(IJavaScriptClassExtension* ext);
    bool DoesObjectFromExtensionExist(IJavaScriptClassExtension* ext);

private:
    bool                       CompileModuleFromFile(const std::filesystem::path& path);
    v8::MaybeLocal<v8::Module> CompileModule(const std::filesystem::path& path);
};

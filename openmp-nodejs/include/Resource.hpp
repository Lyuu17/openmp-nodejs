
#pragma once

#include "common.hpp"
#include "IJavaScriptClassExtension.hpp"

#include <sdk.hpp>

class Resource;

using ResourcePtr = std::unique_ptr<Resource>;

class Resource {
public:
    const std::filesystem::path m_folderPath;
    const std::string           m_folderName;
    const std::string           m_packageBuf;

    bool m_envStarted = false;

    std::string m_name;

    node::ArrayBufferAllocator* m_allocator;
    v8::Isolate*                m_isolate;
    node::IsolateData*          m_nodeData = nullptr;
    node::Environment*          m_env      = nullptr;
    uv_loop_t*                  m_uvLoop   = nullptr;
    v8::Global<v8::Context>     m_context;
    v8::Global<v8::Value>       m_exports;
    v8::Global<v8::Object>      m_asyncResource;
    node::async_context         m_asyncContext;

    std::unordered_map<std::filesystem::path, v8::Global<v8::Module>>      m_modules;
    std::unordered_map<std::string, std::vector<v8::Global<v8::Function>>> m_listeners;

    std::unordered_map<IJavaScriptClassExtension*, v8::Global<v8::Object>> m_objectsFromExtension;

    Resource(node::MultiIsolatePlatform* platform, const std::filesystem::path& folderPath, const std::string& folderName, const std::string& packageJsonBuf);
    ~Resource();

    Resource(Resource&)            = delete;
    Resource& operator=(Resource&) = delete;

    inline v8::Local<v8::Context> GetContext() const
    {
        return m_context.Get(m_isolate);
    }

    void Start(node::MultiIsolatePlatform* platform, node::Environment* parentEnv);
    void Stop();
    void OnTick(node::MultiIsolatePlatform* platform);
    void AddListener(const std::string& name, v8::Local<v8::Function> listener);
    void Emit(const std::string& name, const std::vector<v8::Local<v8::Value>>& values, bool waitAsync = false);
    void AddFunction(const std::string& name, v8::FunctionCallback cb, void* userdata = nullptr);
    void ReportException(v8::TryCatch* try_catch);
    void ThrowException(const std::string& text);
    void Exec(std::function<void(Resource* resource)> func);

    inline v8::Local<v8::Object> ObjectFromExtension(IJavaScriptClassExtension* ext)
    {
        assert(ext);

        v8::EscapableHandleScope handle_scope(m_isolate);

        auto               context = m_isolate->GetCurrentContext();
        v8::Context::Scope context_scope(context);

        if (m_objectsFromExtension.contains(ext))
            return handle_scope.Escape(m_objectsFromExtension.at(ext).Get(m_isolate));

        auto v8obj = ext->CreateJavaScriptObject();

        m_objectsFromExtension[ext].Reset(m_isolate, v8obj);

        return handle_scope.Escape(m_objectsFromExtension.at(ext).Get(m_isolate));
    }

    void RemoveExtension(IJavaScriptClassExtension* ext);
    bool DoesObjectFromExtensionExist(IJavaScriptClassExtension* ext);
};

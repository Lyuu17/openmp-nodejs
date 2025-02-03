
#include "NodejsComponent.hpp"

#include "components/PlayerComponent.hpp"

#include "ResourceManager.hpp"

NodejsComponent* NodejsComponent::m_instance = nullptr;

NodejsComponent::~NodejsComponent()
{
    if (m_core)
    {
        m_core->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
        m_core->getPlayers().getPlayerTextDispatcher().removeEventHandler(this);
        m_core->getEventDispatcher().removeEventHandler(this);
    }
}

StringView NodejsComponent::componentName() const
{
    return "nodejs";
}

SemanticVersion NodejsComponent::componentVersion() const
{
    return { 0, 0, 0, 0 };
}

void NodejsComponent::onLoad(ICore* core)
{
    m_core = core;

    m_core->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
    m_core->getPlayers().getPlayerTextDispatcher().addEventHandler(this);

    auto result = node::InitializeOncePerProcess({}, { node::ProcessInitializationFlags::kDisableNodeOptionsEnv, node::ProcessInitializationFlags::kDisableCLIOptions });

    m_platform.reset(result->platform());

    m_bufferAllocator = node::CreateArrayBufferAllocator();

    ResourceManager::LoadResourcesFromPath("resources");
}

void NodejsComponent::free()
{
    ResourceManager::Deinit();

    v8::V8::Dispose();
    v8::V8::DisposePlatform();

    node::FreeArrayBufferAllocator(m_bufferAllocator);

    delete this;
}

void NodejsComponent::reset()
{
}

void NodejsComponent::onTick(std::chrono::microseconds elapsed, std::chrono::steady_clock::time_point now)
{
}

void NodejsComponent::onIncomingConnection(IPlayer& player, StringView ipAddress, unsigned short port)
{
    player.addExtension(new PlayerComponent(m_core->getPlayers().get(player.getID())), true);

    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::String>  ip          = v8::String::NewFromUtf8(resource->m_isolate, ipAddress.data()).ToLocalChecked();
        v8::Local<v8::Integer> port_       = v8::Integer::New(resource->m_isolate, (int32_t)port);

        resource->Emit("onIncomingConnection", { v8objPlayer, ip, port_ });
    });
}

void NodejsComponent::onPlayerConnect(IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerConnect", { v8objPlayer });
    });
}

void NodejsComponent::onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object>  v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::Integer> reason_     = v8::Integer::New(resource->m_isolate, (int32_t)reason);

        resource->Emit("onPlayerDisconnect", { v8objPlayer, reason_ });
    });
}

void NodejsComponent::onPlayerClientInit(IPlayer& player)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));

        resource->Emit("onPlayerClientInit", { v8objPlayer });
    });
}

bool NodejsComponent::onPlayerText(IPlayer& player, StringView message)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::String> message_    = v8::String::NewFromUtf8(resource->m_isolate, message.data()).ToLocalChecked();

        resource->Emit("onPlayerText", { v8objPlayer, message_ });
    });

    return true; // TODO
}

bool NodejsComponent::onPlayerCommandText(IPlayer& player, StringView message)
{
    ResourceManager::Exec([&](Resource* resource) {
        v8::Local<v8::Object> v8objPlayer = resource->ObjectFromExtension(queryExtension<PlayerComponent>(player));
        v8::Local<v8::String> message_    = v8::String::NewFromUtf8(resource->m_isolate, message.data()).ToLocalChecked();

        resource->Emit("onPlayerCommandText", { v8objPlayer, message_ });
    });

    return true; // TODO
}

ICore* NodejsComponent::getCore()
{
    return m_core;
}

NodejsComponent* NodejsComponent::getInstance()
{
    if (m_instance == nullptr)
        m_instance = new NodejsComponent();

    return m_instance;
}

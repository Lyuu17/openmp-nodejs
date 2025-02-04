
#include <sdk.hpp>

#include "extensions/DbExtension.hpp"

#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include "NodejsComponent.hpp"

DbExtension::DbExtension(ICore* core, IDatabasesComponent* databases)
    : m_core(core)
    , m_databases(databases)
{
}

DbExtension::~DbExtension()
{
    for (auto conn : m_databaseConnections)
        m_databases->close(*conn);

    m_databaseConnections.clear();
}

void DbExtension::freeExtension()
{
}

void DbExtension::reset()
{
}

void DbExtension::InitFunctions(Resource* resource)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();

    context->Global()->Set(context, Utils::v8Str("db_open"), v8::Function::New(context, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto dbExtension = queryExtension<DbExtension>(NodejsComponent::getInstance()->getCore());
        if (!dbExtension)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        auto context = info.GetIsolate()->GetCurrentContext();

        auto v8path = info[0]->ToString(context);
        if (v8path.IsEmpty())
        {
            info.GetReturnValue().SetNull();
            return;
        }

        auto conn = dbExtension->m_databases->open(Utils::strV8(v8path.ToLocalChecked()));

        dbExtension->m_databaseConnections.push_back(conn);

        auto v8obj = v8::Object::New(info.GetIsolate());

#define SET_FUNCTION(f, func) v8obj->Set(context, Utils::v8Str(f), v8::Function::New(context, func, v8::External::New(info.GetIsolate(), conn)).ToLocalChecked());

        SET_FUNCTION("close", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            auto conn = (IDatabaseConnection*)info.Data().As<v8::External>()->Value();

            conn->close();
        });
    }, v8::External::New(isolate, resource))
                                                                 .ToLocalChecked());
}

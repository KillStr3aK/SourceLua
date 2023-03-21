#include "shared.h"

#define SET_RUNTIME_VALUE(VARIABLE, VALUE) this->SetGlobalVariable(VARIABLE, VALUE)

LuaRuntime::LuaRuntime(LuaScript* script)
{
    this->m_pScript = script;
    this->m_instanceId = rand();

    if (this->m_state == NULL)
    {
        script->SetState(LoadScriptResult::Unknown);
    }
}

void LuaRuntime::InitializeEnvironment(void)
{
#ifdef SOURCEMOD_BUILD
    SET_RUNTIME_VALUE("MaxClients", playerhelpers->GetMaxClients());
    SET_RUNTIME_VALUE("MAXPLAYERS", SM_MAXPLAYERS);

    this->AddFunction<void, std::string>("LogMessage", [&](std::string msg) { this->GetScript()->LogMessage(msg.c_str()); });
    this->AddFunction<void, std::string>("LogError", [&](std::string error) { this->GetScript()->LogError(error.c_str()); });
#endif
}

LuaScript* LuaRuntime::GetScript(void)
{
    return this->m_pScript;
}

int LuaRuntime::GetInstanceID(void)
{
    return this->m_instanceId;
}

LuaRef LuaRuntime::GetGlobalByName(const char* name)
{
    return luabridge::getGlobal(this->m_state, name);
}

LuaRef LuaRuntime::GetFunctionByName(const char* name)
{
    return this->GetGlobalByName(name);
}

template<typename T>
void LuaRuntime::SetGlobalVariable(const char* name, T value)
{
    luabridge::setGlobal(this->m_state, value, name);
}

template<class ReturnType, class... Params>
void LuaRuntime::AddFunction(const char* name, std::function<ReturnType(Params...)> function)
{
    luabridge::getGlobalNamespace(this->m_state)
        .addFunction(name, function);
}
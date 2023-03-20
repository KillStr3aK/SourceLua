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

void LuaRuntime::SetGlobalVariable(const char* name, int value)
{
    char szBuffer[32];
    sprintf(szBuffer, "%s = %d;", name, value);
    this->ExecuteString(szBuffer);
}

void LuaRuntime::SetGlobalVariable(const char* name, unsigned int value)
{
    char szBuffer[65];
    sprintf(szBuffer, "%s = %u;", name, value);
    this->ExecuteString(szBuffer);
}

void LuaRuntime::SetGlobalVariable(const char* name, const char* value)
{
    char szBuffer[32];
    sprintf(szBuffer, "%s = \"%s\";", name, value);
    this->ExecuteString(szBuffer);
}

void LuaRuntime::SetGlobalVariable(const char* name, bool value)
{
    char szBuffer[32];
    sprintf(szBuffer, "%s = %d;", name, value);
    this->ExecuteString(szBuffer);
}

void LuaRuntime::SetGlobalVariable(const char* name, float value)
{
    char szBuffer[32];
    sprintf(szBuffer, "%s = %f;", name, value);
    this->ExecuteString(szBuffer);
}

template<class ReturnType, class... Params>
void LuaRuntime::AddFunction(const char* name, std::function<ReturnType(Params...)> function)
{
    luabridge::getGlobalNamespace(this->m_state)
        .addFunction(name, function)
    .endNamespace();
}
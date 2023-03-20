#include "shared.h"

LuaRuntime::LuaRuntime(LuaScript* script)
{
    this->m_pScript = script;
    this->m_instanceId = rand();

    if (this->m_state == NULL)
    {
        script->SetState(LoadScriptResult::Unknown);
    }
}

LuaScript* LuaRuntime::GetScript(void)
{
    return this->m_pScript;
}

LuaRef LuaRuntime::GetGlobalByName(const char* name)
{
    return luabridge::getGlobal(this->m_state, name);
}

LuaRef LuaRuntime::GetFunctionByName(const char* name)
{
    return this->GetGlobalByName(name);
}
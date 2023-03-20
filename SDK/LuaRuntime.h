#pragma once

#ifndef LUA_RUNTIME_H
#define LUA_RUNTIME_H

#include "shared.h"

class LuaRuntime : public ILuaEngine
{
public:
    LuaRuntime(LuaScript* script);

    LuaScript* GetScript(void);

    LuaRef GetGlobalByName(const char* name);
    LuaRef GetFunctionByName(const char* name);

private:
    LuaScript* m_pScript;
    int m_instanceId;
};

#endif
#pragma once

#ifndef LUA_RUNTIME_H
#define LUA_RUNTIME_H

#include "shared.h"

class LuaRuntime final : public ILuaEngine
{
public:
    LuaRuntime(LuaScript* script);

    void InitializeEnvironment(void);

#ifdef SOURCEMOD_BUILD
    void InitializeSourceMod(void);
#endif

    LuaScript* GetScript(void);
    int GetInstanceID(void);

    LuaRef GetGlobalByName(const char* name);
    LuaRef GetFunctionByName(const char* name);

    // LuaBridge doesn't really support setting global variables ( http://vinniefalco.github.io/LuaBridge/Manual.html#s1.1 )
    // This works on any type specialized by `Stack`, including `LuaRef` and its table proxies.
    template<typename T>
    void SetGlobalVariable(const char* name, T value);

    template<class ReturnType, class... Params>
    void AddFunction(const char* name, std::function<ReturnType(Params...)> function);

private:
    LuaScript* m_pScript;
    int m_instanceId;
};

#endif
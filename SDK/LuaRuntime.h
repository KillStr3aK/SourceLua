#pragma once

#ifndef LUA_RUNTIME_H
#define LUA_RUNTIME_H

#include "shared.h"

class LuaRuntime : public ILuaEngine
{
public:
    LuaRuntime(LuaScript* script);

    void InitializeEnvironment(void);

    LuaScript* GetScript(void);
    int GetInstanceID(void);

    LuaRef GetGlobalByName(const char* name);
    LuaRef GetFunctionByName(const char* name);

    // LuaBridge doesn't support setting global variables ( http://vinniefalco.github.io/LuaBridge/Manual.html#s1.1 )
    // Only primitive types are supported by this methods
    void SetGlobalVariable(const char* name, int value);
    void SetGlobalVariable(const char* name, unsigned int value);
    void SetGlobalVariable(const char* name, const char* value);
    void SetGlobalVariable(const char* name, bool value);
    void SetGlobalVariable(const char* name, float value);

    template<class ReturnType, class... Params>
    void AddFunction(const char* name, std::function<ReturnType(Params...)> function);

private:
    LuaScript* m_pScript;
    int m_instanceId;
};

#endif
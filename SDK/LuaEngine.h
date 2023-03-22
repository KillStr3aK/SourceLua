#pragma once

#ifndef LUAENGINE_H
#define LUAENGINE_H

#include <iostream>

#include "Libs/Lua/lua.hpp"
#include "Libs/LuaBridge/LuaBridge.h"

#define LUA_ENGINE_NO_ERROR 0

class ILuaEngine
{
public:
    ILuaEngine(void);
	virtual ~ILuaEngine(void);

	lua_State* GetState(void);
    int GetRunState(void);

	virtual void LoadFile(const char* file);
	virtual void ExecuteString(const char* expression);

	virtual void Reset(void);
    virtual const char* GetLastError(void);

private:
    void ErrorHandler(int state);
    void ErrorHandler(int state, const char* error);

protected:

	lua_State* m_state;
    const char* m_error;
    int m_runState;
};

class LuaEngine : public CSingleton<ILuaEngine>
{
	friend ILuaEngine* CSingleton<ILuaEngine>::GetInstance(void);
};

#endif
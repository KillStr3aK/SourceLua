#pragma once

#ifndef LUAENGINE_H
#define LUAENGINE_H

#include <iostream>

#include "Libs/Lua/lua.hpp"
#include "Libs/LuaBridge/LuaBridge.h"

class ILuaEngine
{
public:
    ILuaEngine(void);
	virtual ~ILuaEngine(void);

	lua_State* GetState(void);

	virtual void LoadFile(const char* file);
	virtual void ExecuteString(const char* expression);

	virtual void Reset(void);

    virtual void ErrorHandler(int state);
    virtual const char* GetLastError(void);

protected:
	lua_State* m_state;
    const char* m_error;
};

class LuaEngine : public CSingleton<ILuaEngine>
{
	friend ILuaEngine* CSingleton<ILuaEngine>::GetInstance(void);
};

#endif
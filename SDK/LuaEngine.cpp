#include "shared.h"

ILuaEngine::ILuaEngine(void) : m_state(luaL_newstate()), m_runState(0)
{
	luaL_openlibs(this->m_state);
}

ILuaEngine::~ILuaEngine(void)
{
    if (this->m_state)
    {
        lua_close(this->m_state);
        this->m_state = nullptr;
    }
}

lua_State* ILuaEngine::GetState()
{
	return this->m_state;
}

void ILuaEngine::LoadFile(const char* file)
{
	if (!file || !this->m_state)
	{
        this->ErrorHandler(1, "File is invalid, or lua state does not exists.");
        return;
    }

	int state = luaL_dofile(this->m_state, file);

    if (state)
    {
        this->ErrorHandler(state);
    }
}

void ILuaEngine::ExecuteString(const char* expression)
{
	if (!expression || !this->m_state)
	{
        this->ErrorHandler(1, "Expression is invalid, or lua state does not exists.");
        return;
    }

	int state = luaL_dostring(this->m_state, expression);

    if (state)
    {
        this->ErrorHandler(state);
    }
}

void ILuaEngine::Reset(void)
{
	if (this->m_state)
	{
		lua_close(this->m_state);
	}

	this->m_state = luaL_newstate();
	luaL_openlibs(this->m_state);
}

void ILuaEngine::ErrorHandler(int state)
{
	if (state)
	{
		this->m_error = lua_tostring(m_state, -1);
		lua_pop(m_state, 1);
	}

    this->m_runState = state;
}

void ILuaEngine::ErrorHandler(int state, const char* error)
{
    this->m_error = error;
    this->m_runState = state;
}

const char* ILuaEngine::GetLastError(void)
{
    return this->m_error;
}

int ILuaEngine::GetRunState(void)
{
    return this->m_runState;
}
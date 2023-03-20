#include "shared.h"

ILuaEngine::ILuaEngine(void) : m_state(luaL_newstate())
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
		return;

	int state = luaL_dofile(this->m_state, file);
	this->ErrorHandler(state);
}

void ILuaEngine::ExecuteString(const char* expression)
{
	if (!expression || !this->m_state)
		return;

	int state = luaL_dostring(this->m_state, expression);
	this->ErrorHandler(state);
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
}

const char* ILuaEngine::GetLastError(void)
{
    return this->m_error;
}
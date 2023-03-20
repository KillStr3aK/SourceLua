#include "shared.h"

auto LuaScripting::Initialize(void) -> void
{
	Console::WriteLine("Initializing scripting environment..");
	Console::WriteLine("LuaEngine: 0x%p", LuaEngine::GetInstance());
	Console::WriteLine("Initialized!");

    Console::WriteLine("Loading scripts..");
    // LuaScriptManager::GetInstance()->LoadScripts(LIBS_FOLDER);
    LuaScriptManager::GetInstance()->LoadScripts(SCRIPTS_FOLDER);
}

auto LuaScripting::Release(void) -> void
{
    Console::WriteLine("Releasing...");
    LuaEngine::GetInstance()->~ILuaEngine();
    Console::WriteLine("Released!");
}
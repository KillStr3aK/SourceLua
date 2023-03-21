#include "shared.h"

auto LuaScripting::Initialize(void) -> void
{
	Console::WriteLine("Initializing scripting environment..");
	Console::WriteLine("LuaEngine: 0x%p", LuaEngine::GetInstance());
	Console::WriteLine("Initialized!");

    Console::WriteLine("Loading scripts..");

    try {
        LuaScriptManager::GetInstance()->LoadScripts(SCRIPTS_FOLDER);
    } catch (std::exception e)
    {
        Console::Error(e.what());
    }
}

auto LuaScripting::Release(void) -> void
{
    Console::WriteLine("Releasing...");
    LuaEngine::GetInstance()->~ILuaEngine();
    Console::WriteLine("Released!");
}
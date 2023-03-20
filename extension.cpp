#include "SDK/shared.h"

SourceLua SourceLua;
SMEXT_LINK(&SourceLua);

bool SourceLua::SDK_OnLoad(char* error, size_t maxlength, bool late)
{
    LuaScripting::Initialize();
    return true;
}

void SourceLua::SDK_OnUnload(void)
{
    LuaScripting::Release();
}

void SourceLua::SDK_OnAllLoaded(void)
{
    playerhelpers->AddClientListener(LuaScriptManager::GetInstance());
}
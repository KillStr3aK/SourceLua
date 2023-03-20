#include "shared.h"

LuaScript::LuaScript(const char* scriptName) : m_state(LoadScriptResult::Initializing), m_pRuntime(NULL)
{
    strcpy(this->m_scriptName, scriptName);
    this->m_pManifest = new LuaScriptManifest(this);
    this->m_pRuntime = new LuaRuntime(this);

    if (this->GetState() != LoadScriptResult::ManifestLoaded)
        return;

    const auto files = this->GetManifest()->GetFiles();

    for (const auto file : files)
    {
        char filePath[PLATFORM_MAX_PATH];

#ifdef SOURCEMOD_BUILD
        g_pSM->BuildPath(Path_SM, filePath, sizeof(filePath), "%s/%s/%s", SCRIPTS_FOLDER, this->m_pManifest->GetScriptName(), file.c_str());
#endif

#ifdef SOURCEMOD_BUILD
        const char* fileExtension = libsys->GetFileExtension(filePath);
#else
        const char* fileExtension = "lua";
#endif

        if (!fileExtension || strcmp(fileExtension, "lua") != 0)
        {
            Console::Error("Script files must have the \".lua\" extension, skipping file '%s'", file);
            continue;
        }

        this->GetRuntime()->LoadFile(filePath);
    }

    this->SetState(LoadScriptResult::Success);
}

LoadScriptResult LuaScript::GetState(void) const
{
    return this->m_state;
}

void LuaScript::SetState(LoadScriptResult state)
{
    this->m_state = state;
}

const char* LuaScript::GetName(void) const
{
    return this->m_scriptName;
}

LuaScriptManifest* LuaScript::GetManifest(void) const
{
    return this->m_pManifest;
}

bool LuaScript::IsRunning(void)
{
    return this->m_state == LoadScriptResult::Running;
}

LuaRuntime* LuaScript::GetRuntime(void) const
{
    return this->m_pRuntime;
}

void LuaScript::ExceptionHandler(LuaException const& e)
{
    Console::Error("SCRIPT ERROR: %s\n%s", this->GetManifest()->GetScriptName(), e.what());
}

void LuaScript::CallFunction(const char* forwardName)
{
    LuaRef luaFunc = this->m_pRuntime->GetFunctionByName(forwardName);

    if (luaFunc)
    {
        try {
            luaFunc();
        } catch(LuaException const& e)
        {
            this->ExceptionHandler(e);
        }
    }
}

void LuaScript::OnScriptStart(void)
{
    this->CallFunction("OnScriptStart");
}

void LuaScript::OnScriptEnd(void)
{
    this->CallFunction("OnScriptEnd");
}

void LuaScript::OnAllScriptsLoaded(void)
{
    this->CallFunction("OnAllScriptsLoaded");
}

void LuaScript::OnAllPluginsLoaded(void)
{
    this->CallFunction("OnAllPluginsLoaded");
}

void LuaScript::OnMapStart(void)
{
    this->CallFunction("OnMapStart");
}

void LuaScript::OnMapEnd(void)
{
    this->CallFunction("OnMapEnd");
}
#pragma once

#ifndef LUA_SCRIPT_MANAGER_H
#define LUA_SCRIPT_MANAGER_H

#include <map>

#include "shared.h"

#define LIBS_FOLDER "lua/libs"
#define SCRIPTS_FOLDER "lua/scripts"

#ifdef SOURCEMOD_BUILD
#define TAG SMEXT_CONF_LOGTAG
#else
#define TAG "Lua"
#endif

class LuaScript;
class LuaScriptManifest;

#ifdef SOURCEMOD_BUILD
class LuaScriptManager : public CSingleton<LuaScriptManager>, public IRootConsoleCommand
#else
class LuaScriptManager : public CSingleton<LuaScriptManager>
#endif
{
    friend LuaScriptManager* CSingleton<LuaScriptManager>::GetInstance(void);
public:
    LuaScriptManager(void);
    ~LuaScriptManager(void);

    void LoadScripts(const char* relativeDirectoryPath);
    void UnloadScripts(void);

    void LoadScript(const char* scriptName);
    void UnloadScript(const char* name);
    void UnloadScript(LuaScript* script);

    LuaScript* GetScriptByName(const char* scriptName);

    int GetScriptCount(void);

#ifdef SOURCEMOD_BUILD
    void OnRootConsoleCommand(const char *cmdname, const ICommandArgs *args) override;
#endif
private:
    std::map<std::string, LuaScript*> m_scripts;
};

#endif
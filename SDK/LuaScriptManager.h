#pragma once

#ifndef LUA_SCRIPT_MANAGER_H
#define LUA_SCRIPT_MANAGER_H

#include <map>

#include "shared.h"
#include "sourcemod/ConsoleMenu.h"

#define LIBS_FOLDER "lua/libs"
#define SCRIPTS_FOLDER "lua/scripts"

#define MENU_CMD "scripts"
#define MENU_NAME "SourceLua Scripts Menu"

#ifdef SOURCEMOD_BUILD
#define TAG SMEXT_CONF_LOGTAG
#else
#define TAG "Lua"
#endif

#define DEFINE_VIRTUAL_OVERRIDE(NAME, PARAMS, NAMES) void NAME(PARAMS) override { for (const auto& script : this->m_scripts) { if (script.second->IsRunning()) { script.second->NAME(NAMES); } } }
#define DEFINE_VIRTUAL_OVERRIDE_RETURN(RETTYPE, NAME, PARAMS, NAMES, RETVALUE) RETTYPE NAME(PARAMS) override { for (const auto& script : this->m_scripts) { if (script.second->IsRunning()) { script.second->NAME(NAMES); } } return RETVALUE; }

class LuaScript;
class LuaScriptManifest;

#ifdef SOURCEMOD_BUILD
class LuaScriptManager : public CSingleton<LuaScriptManager>, public ConsoleMenu, public IClientListener
#else
class LuaScriptManager : public CSingleton<LuaScriptManager>
#endif
{
    friend LuaScriptManager* CSingleton<LuaScriptManager>::GetInstance(void);
public:
#ifdef SOURCEMOD_BUILD
    LuaScriptManager(void) : ConsoleMenu(MENU_NAME)
    {
        ConsoleMenuCategory* category = this->RegisterMenuCategory(MENU_CMD, "Manage Scripts");

        category->RegisterCommand("info", "Information about a script", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });

        category->RegisterCommand("list", "Show loaded scripts", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });

        category->RegisterCommand("load", "Load a script", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });

        category->RegisterCommand("refresh", "Reloads/refreshes all scripts in the scripts folder", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });

        category->RegisterCommand("reload", "Reloads a script", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });

        category->RegisterCommand("unload", "Unload a script", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });

        category->RegisterCommand("unload_all", "Unloads all scripts", [](ConsoleMenuCommand* command, const ICommandArgs* args)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command->GetName());
        });
    }
#endif

    ~LuaScriptManager(void);

    void LoadScripts(const char* relativeDirectoryPath);
    void UnloadScripts(void);

    void LoadScript(const char* scriptName);
    void UnloadScript(const char* name);
    void UnloadScript(LuaScript* script);

    void LoadLibraries(LuaScript* script);
    LuaScript* GetScriptByName(const char* scriptName);
    int GetScriptCount(void);

    DEFINE_VIRTUAL_OVERRIDE_RETURN(bool, InterceptClientConnect, P(int client, char* error, size_t maxlength), P(client, error, maxlength), true);
    DEFINE_VIRTUAL_OVERRIDE_RETURN(bool, OnClientPreAdminCheck, int client, client, true);

    DEFINE_VIRTUAL_OVERRIDE(OnClientConnected, int client, client);
    DEFINE_VIRTUAL_OVERRIDE(OnClientPutInServer, int client, client);
    DEFINE_VIRTUAL_OVERRIDE(OnClientDisconnecting, int client, client);
    DEFINE_VIRTUAL_OVERRIDE(OnClientDisconnected, int client, client);
    DEFINE_VIRTUAL_OVERRIDE(OnClientPostAdminCheck, int client, client);
    DEFINE_VIRTUAL_OVERRIDE(OnClientSettingsChanged, int client, client);
    DEFINE_VIRTUAL_OVERRIDE(OnClientAuthorized, P(int client, const char* authstring), P(client, authstring));

    DEFINE_VIRTUAL_OVERRIDE(OnMaxPlayersChanged, int newvalue, newvalue);
    DEFINE_VIRTUAL_OVERRIDE(OnServerActivated, int max_clients, max_clients);
private:
    std::map<std::string, LuaScript*> m_scripts;
};

#endif
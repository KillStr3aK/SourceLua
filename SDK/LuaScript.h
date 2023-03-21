#pragma once

#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "shared.h"

#define LUA_FORWARD(NAME) void NAME(void) { LuaScript::CallFunction(#NAME); }
#define LUA_FUNCTION(RET, NAME, PARAMS, TYPES, NAMES) RET NAME(PARAMS) { LuaScript::CallFunction<TYPES>(#NAME, NAMES); }

enum class LoadScriptResult : int
{
    // Script is initializing
    Initializing,

    // Script has been loaded
    Success,

    // Script has been started
    Running,

    // Manifest is missing
    ManifestMissing,

    // Unable to parse manifest
    ManifestParseError,

    // Manifest file has been loaded
    ManifestLoaded,

    // Script has missing files
    NoSuchFile,

    // Unable to parse script
    ParseError,

    // Unknown state
    Unknown
};

enum class ScriptState : int
{
    // Script is stopped
    Stopped,

    // Script is running
    Running,

    // Script errored
    Errored,

    // Unknown state
    Unknown
};

class LuaScript
{
private:
    LuaScript(void) = delete;
public:
    LuaScript(const char* scriptName);

    LuaRuntime* GetRuntime(void) const;
    LuaScriptManifest* GetManifest(void) const;
    const char* GetName(void) const;

    LoadScriptResult GetState(void) const;
    void SetState(LoadScriptResult state);

    void ExceptionHandler(LuaException const& e);

    bool IsRunning(void);

    void LogMessage(const char* msg);
    void LogError(const char* error);

    void Initialize(void);

    LUA_FORWARD(OnScriptStart);
    LUA_FORWARD(OnScriptEnd);

    LUA_FORWARD(OnAllScriptsLoaded);

#ifdef SOURCEMOD_BUILD
    void OnMaxPlayersChanged(int newvalue)
    {
        this->SyncMaxClients(newvalue);
    }

    void OnServerActivated(int max_clients)
    {
        this->SyncMaxClients(max_clients);
    }

    LUA_FUNCTION(void, OnClientPostAdminCheck, int client, int, client);
    LUA_FUNCTION(void, OnClientPreAdminCheck, int client, int, client);
    LUA_FUNCTION(void, OnClientDisconnected, int client, int, client);
    LUA_FUNCTION(void, OnClientDisconnecting, int client, int, client);
    LUA_FUNCTION(void, OnClientPutInServer, int client, int, client);
    LUA_FUNCTION(void, OnClientConnected, int client, int, client);
    LUA_FUNCTION(void, OnClientSettingsChanged, int client, int, client);
    LUA_FUNCTION(void, InterceptClientConnect, P(int client, char *error, size_t maxlength), P(int, char*, size_t), P(client, error, maxlength));
    LUA_FUNCTION(void, OnClientAuthorized, P(int client, const char* authstring), P(int, const char*), P(client, authstring));

    LUA_FORWARD(OnAllPluginsLoaded);

    LUA_FORWARD(OnMapInit);
    LUA_FORWARD(OnMapStart);
    LUA_FORWARD(OnMapEnd);
#endif

#ifdef SOURCEMOD_BUILD
    void SyncMaxClients(int maxClients);
#endif

private:
    void CallFunction(const char* functionName);

    template <class... Args>
    void CallFunction(const char* functionName, Args... args)
    {
        LuaRef luaFunc = this->m_pRuntime->GetFunctionByName(functionName);

        if (luaFunc)
        {
            try {
                luaFunc(args...);
            } catch(LuaException const& e)
            {
                this->ExceptionHandler(e);
            }
        }
    }

    char m_scriptName[PLATFORM_MAX_PATH];
    LoadScriptResult m_state;
    ScriptState m_runState;

    LuaScriptManifest* m_pManifest;
    LuaRuntime* m_pRuntime;
};

#endif
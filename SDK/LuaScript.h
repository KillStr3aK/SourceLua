#pragma once

#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "shared.h"

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

    void OnScriptStart(void);
    void OnScriptEnd(void);

    void OnAllScriptsLoaded(void);
    void OnAllPluginsLoaded(void);

    void OnMapStart(void);
    void OnMapEnd(void);

private:
    void CallFunction(const char* forwardName);

    char m_scriptName[PLATFORM_MAX_PATH];
    LoadScriptResult m_state;
    ScriptState m_runState;

    LuaScriptManifest* m_pManifest;
    LuaRuntime* m_pRuntime;
};

#endif
#pragma once

#ifndef LUA_SCRIPT_MANIFEST_H
#define LUA_SCRIPT_MANIFEST_H

#define MANIFEST_FILE "manifest.json"

#include <fstream>
#include <vector>

class LuaScript;

typedef struct ScriptInfo_t
{
    std::string Name;
    std::string Author;
    std::string Description;
    std::string Version;
    std::string Url;

    std::string GetName(void) { return this->Name; }
    std::string GetAuthor(void) { return this->Author; }
    std::string GetDescription(void) { return this->Description; }
    std::string GetVersion(void) { return this->Version; }
    std::string GetUrl(void) { return this->Url; }
} ScriptInfo;

class LuaScriptManifest
{
private:
    LuaScriptManifest(void);
public:
    LuaScriptManifest(LuaScript* script);

    LuaScript* GetScript(void);
    ScriptInfo* GetPublicInfo(void);
    std::vector<std::string> GetFiles(void);

    const char* GetScriptName(void);
    const char* GetPath(void);
    bool Exists(void);
    void Load(void);

private:
    ScriptInfo m_info;
    LuaScript* m_pScript;
    std::vector<std::string> m_scripts;
};

#endif
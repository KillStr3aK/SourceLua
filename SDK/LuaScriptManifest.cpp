#include "shared.h"

LuaScriptManifest::LuaScriptManifest(LuaScript* script) : m_info(ScriptInfo())
{
    this->m_pScript = script;

    if (!this->Exists())
    {
        this->m_pScript->SetState(LoadScriptResult::ManifestMissing);
        return;
    }

    char manifestFilePath[PLATFORM_MAX_PATH];

#ifdef SOURCEMOD_BUILD
    g_pSM->BuildPath(Path_SM, manifestFilePath, sizeof(manifestFilePath), "%s/%s/%s", SCRIPTS_FOLDER, this->GetScriptName(), MANIFEST_FILE);
#endif

    std::ifstream buffer(manifestFilePath);

    try {
        json manifest = json::parse(buffer);
        this->m_info.Name = manifest["myinfo"]["name"];
        this->m_info.Author = manifest["myinfo"]["author"];
        this->m_info.Description = manifest["myinfo"]["description"];
        this->m_info.Version = manifest["myinfo"]["version"];
        this->m_info.Url = manifest["myinfo"]["url"];

        this->m_scripts = manifest["scripts"].get<std::vector<std::string>>();
        this->m_pScript->SetState(LoadScriptResult::ManifestLoaded);
    } catch(json::parse_error& e)
    {
        Console::Error(e.what());
        this->m_pScript->SetState(LoadScriptResult::ManifestParseError);
    }
}

LuaScript* LuaScriptManifest::GetScript(void)
{
    return this->m_pScript;
}

const char* LuaScriptManifest::GetScriptName(void)
{
    return this->m_pScript->GetName();
}

ScriptInfo* LuaScriptManifest::GetPublicInfo(void)
{
    return &this->m_info;
}

std::vector<std::string> LuaScriptManifest::GetFiles(void)
{
    return this->m_scripts;
}

bool LuaScriptManifest::Exists(void)
{
    char manifestFilePath[PLATFORM_MAX_PATH];

#ifdef SOURCEMOD_BUILD
    g_pSM->BuildPath(Path_SM, manifestFilePath, sizeof(manifestFilePath), "%s/%s/%s", SCRIPTS_FOLDER, this->GetScriptName(), MANIFEST_FILE);
    return libsys->PathExists(manifestFilePath);
#else
    // normal implementation
#endif
}
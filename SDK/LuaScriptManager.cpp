#include "shared.h"

LuaScriptManager::~LuaScriptManager(void)
{
    this->UnloadScripts();
}

void LuaScriptManager::UnloadScripts(void)
{
    for (const auto& script : this->m_scripts)
    {
        this->UnloadScript(script.second);
    }

    this->m_scripts.clear();
}

void LuaScriptManager::UnloadScript(const char* name)
{
    LuaScript* script = this->GetScriptByName(name);

    if (script != NULL)
    {
        this->UnloadScript(script);
    }
}

void LuaScriptManager::UnloadScript(LuaScript* script)
{
    // ?
    if (!script->IsRunning())
    {
        delete script;
        return;
    }

    script->OnScriptEnd();
    delete script;
}

void LuaScriptManager::LoadScript(const char* scriptName)
{
    if (strstr(scriptName, "..") != NULL)
    {
        Console::Error("Cannot load scripts outside the \"%s\" folder", SCRIPTS_FOLDER);
        return;
    }

    LuaScript* script = new LuaScript(scriptName);
    LoadScriptResult result = script->GetState();

    if (result != LoadScriptResult::Success)
    {
        char error[256];

        switch(result)
        {
            case LoadScriptResult::ManifestMissing:
            {
                strcpy(error, "Script manifest not found.");
            } break;

            case LoadScriptResult::ManifestParseError:
            {
                strcpy(error, "Could not parse script manifest file. (ERROR)");
            } break;

            case LoadScriptResult::NoSuchFile:
            {
                strcpy(error, "File does not exists");
            } break;

            case LoadScriptResult::ParseError:
            {
                strcpy(error, "Unable to parse script");
            } break;

            default:
            {
                strcpy(error, "Unknown error happened during script loading.");
            } break;
        }

        Console::Error("Unable to load script '%s' (%s)", scriptName, error);
        delete script;
    } else {
        script->Initialize();
        script->OnScriptStart();
        script->SetState(LoadScriptResult::Running);
        Console::WriteLine("Script '%s' has been loaded", scriptName);

        this->m_scripts.insert(std::pair<std::string, LuaScript*>(scriptName, script));
    }
}

void LuaScriptManager::LoadScripts(const char* relativeDirectoryPath)
{
    char fullpath[PLATFORM_MAX_PATH];

#ifdef SOURCEMOD_BUILD
    g_pSM->BuildPath(Path_SM, fullpath, sizeof(fullpath), relativeDirectoryPath);

    IDirectory* dir = libsys->OpenDirectory(fullpath);

    if (dir == NULL)
    {
        char error[256];
        libsys->GetPlatformError(error, sizeof(error));
        Console::Error("Unable to open directory at path \"%s\" error: %s", relativeDirectoryPath, error);
        return;
    }

    while (dir->MoreFiles())
    {
        if (strcmp(dir->GetEntryName(), ".") == 0 || strcmp(dir->GetEntryName(), "..") == 0)
		{
			dir->NextEntry();
			continue;
		}

        if (dir->IsEntryDirectory())
        {
            this->LoadScript(dir->GetEntryName());
        }

        dir->NextEntry();
    }

    libsys->CloseDirectory(dir);
#else
    // normal implementation
#endif

    for (const auto& script : this->m_scripts)
    {
        if (script.second->IsRunning())
        {
            script.second->OnAllScriptsLoaded();

#ifdef SOURCEMOD_BUILD
            if (g_pSM->IsMapRunning())
            {
                script.second->OnMapStart();
            }
#endif
        }
    }
}

LuaScript* LuaScriptManager::GetScriptByName(const char* scriptName)
{
    if (this->m_scripts.find(scriptName) == this->m_scripts.end())
        return NULL;

    return this->m_scripts[scriptName];
}

int LuaScriptManager::GetScriptCount(void)
{
    return this->m_scripts.size();
}
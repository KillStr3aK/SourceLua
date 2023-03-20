#include "shared.h"

LuaScriptManager::LuaScriptManager(void)
{
    rootconsole->AddRootConsoleCommand3("scripts", "Manage Scripts", this);
}

LuaScriptManager::~LuaScriptManager(void)
{
    rootconsole->RemoveRootConsoleCommand("scripts", this);
    this->UnloadScripts();
}

void LuaScriptManager::UnloadScripts(void)
{
    for (const auto& script : this->m_scripts)
    {
        if (script.second->IsRunning())
        {
            this->UnloadScript(script.second);
        }
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
        return;

    script->OnScriptEnd();
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

            default:
            {
                strcpy(error, "Unknown error happened during script loading.");
            } break;
        }

        Console::Error("Unable to load script '%s' (%s)", scriptName, error);
    } else {
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

LuaScript* LuaScriptManager::GetScriptByName(const char* directoryName)
{
    if (this->m_scripts.find(directoryName) == this->m_scripts.end())
        return NULL;

    return this->m_scripts[directoryName];
}

int LuaScriptManager::GetScriptCount(void)
{
    return this->m_scripts.size();
}

#ifdef SOURCEMOD_BUILD
void LuaScriptManager::OnRootConsoleCommand(const char *cmdname, const ICommandArgs *args)
{
    int argc = args->ArgC();

    if (argc == 2)
    {
        rootconsole->ConsolePrint("SourceLua Scripts Menu:");
        rootconsole->DrawGenericOption("info", "Information about a script");
        rootconsole->DrawGenericOption("list", "Show loaded scripts");
        rootconsole->DrawGenericOption("load", "Load a script");
        rootconsole->DrawGenericOption("refresh", "Reloads/refreshes all scripts in the scripts folder");
        rootconsole->DrawGenericOption("reload", "Reloads a script");
        rootconsole->DrawGenericOption("unload", "Unload a script");
        rootconsole->DrawGenericOption("unload_all", "Unloads all scripts");
        return;
    }

    const char* command = args->Arg(2);

    auto HasExtraParam = [](const char* cmd) -> bool
    {
        return strcmp(cmd, "load") == 0 || strcmp(cmd, "unload") == 0 || strcmp(cmd, "reload") == 0;
    };

    if (!HasExtraParam(command))
    {
        if (strcmp(command, "info") == 0)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        } else if (strcmp(command, "list") == 0)
        {
            int scriptCount = this->GetScriptCount();

            if (scriptCount == 0)
            {
                rootconsole->ConsolePrint("[%s] No scripts loaded", SMEXT_CONF_LOGTAG);
                return;
            }

            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        } else if (strcmp(command, "refresh") == 0)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        } else if (strcmp(command, "unload_all") == 0)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        }
    } else {
        if (argc != 4)
        {
            rootconsole->ConsolePrint("[%s] Usage: sm scripts %, SMEXT_CONF_LOGTAGs <script name>", SMEXT_CONF_LOGTAG, command);
            return;
        }

        const char* param = args->Arg(3);

        if (strcmp(command, "load") == 0)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        } else if (strcmp(command, "unload") == 0)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        } else if (strcmp(command, "reload") == 0)
        {
            rootconsole->ConsolePrint("[%s] Not implemented %s", SMEXT_CONF_LOGTAG, command);
        }
    }
}
#endif

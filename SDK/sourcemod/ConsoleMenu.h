#pragma once

#ifndef CONSOLE_MENU_H
#define CONSOLE_MENU_H

#include <map>

#include "smsdk_ext.h"

enum
{
	COMMAND_MAX_ARGC = 64,
	COMMAND_MAX_LENGTH = 512,
};

class ConsoleMenuCommand;

typedef void(*ConsoleCommandCallback)(ConsoleMenuCommand* command, const ICommandArgs* args);

class NameDesc
{
public:
    NameDesc(const char* name, const char* description)
    {
        strcpy(this->m_name, name);
        strcpy(this->m_description, description);
    }

    const char* GetName(void) { return this->m_name; }
    const char* GetDescription(void) { return this->m_description; }
    void SetName(const char* name) { strcpy(this->m_name, name); }
    void SetDescription(const char* description) { strcpy(this->m_description, description); }

private:
    char m_name[COMMAND_MAX_LENGTH];
    char m_description[COMMAND_MAX_LENGTH];
};

template<typename Key, class Value>
class ClassMap
{
public:
    std::map<Key, Value> GetEntries(void)
    {
        return this->m_map;
    }

    Value Get(Key key)
    {
        if (this->m_map.find(key) == this->m_map.end())
            return NULL;

        return this->m_map[key];
    }
protected:
    std::map<Key, Value> m_map;
};

class ConsoleMenuCommand : public NameDesc
{
public:
    ConsoleMenuCommand(const char* command, const char* description, ConsoleCommandCallback callback) : NameDesc(command, description)
    {
        this->m_callback = callback;
    }

    void InvokeCallback(const ICommandArgs* args)
    {
        this->m_callback(this, args);
    }

private:
    ConsoleCommandCallback m_callback;
};

class ConsoleMenuCategory : public NameDesc, public ClassMap<std::string, ConsoleMenuCommand*>
{
public:
    ConsoleMenuCategory(const char* name, const char* description) : NameDesc(name, description) {}

    ConsoleMenuCommand* RegisterCommand(const char* command, const char* description, ConsoleCommandCallback callback)
    {
        ConsoleMenuCommand* cmd = new ConsoleMenuCommand(command, description, callback);
        this->m_map.insert(std::pair<std::string, ConsoleMenuCommand*>(command, cmd));
        return cmd;
    }
};

class ConsoleMenu : public IRootConsoleCommand, public ClassMap<std::string, ConsoleMenuCategory*>
{
public:
    ConsoleMenu(const char* name) { strcpy(this->m_name, name); }
    ~ConsoleMenu(void)
    {
        for (const auto category : this->GetEntries())
        {
            rootconsole->RemoveRootConsoleCommand(category.second->GetName(), this);
        }
    }

    void SetMenuName(const char* name) { strcpy(this->m_name, name); }
    const char* GetMenuName(void) { return this->m_name; }

    ConsoleMenuCategory* RegisterMenuCategory(const char* name, const char* description)
    {
        ConsoleMenuCategory* category = new ConsoleMenuCategory(name, description);
        this->m_map.insert(std::pair<std::string, ConsoleMenuCategory*>(name, category));
        rootconsole->AddRootConsoleCommand3(name, description, this);
        return category;
    }

    void OnRootConsoleCommand(const char *cmdname, const ICommandArgs *args) override
    {
        int argc = args->ArgC();
        const char* commandName = args->Arg(2);

        ConsoleMenuCategory* category = this->Get(cmdname);
        ConsoleMenuCommand* command = category->Get(commandName);

        if (argc == 2 || command == NULL)
        {
            rootconsole->ConsolePrint("%s:", this->GetMenuName());

            for (const auto command : category->GetEntries())
            {
                rootconsole->DrawGenericOption(command.second->GetName(), command.second->GetDescription());
            }

            return;
        }

        command->InvokeCallback(args);
    }

private:
    char m_name[COMMAND_MAX_LENGTH];
};

#endif
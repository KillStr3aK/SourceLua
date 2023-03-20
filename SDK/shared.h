#include "../extension.h"

#ifndef PLATFORM_MAX_PATH
    #ifdef SOURCEMOD_BUILD
        #define PLATFORM_MAX_PATH MAX_PATH
    #else
        #define PLATFORM_MAX_PATH 260
    #endif
#endif

#include "Libs/json/json.hpp"
using json = nlohmann::json;

#include "Common/CSingleton.h"
#include "Common/Console.h"

#include "LuaEngine.h"
using namespace luabridge;

#include "LuaScriptManifest.h"
#include "LuaScript.h"
#include "LuaRuntime.h"
#include "LuaScriptManager.h"
#include "LuaScripting.h"
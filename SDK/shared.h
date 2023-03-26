#include "../extension.h"
#include "sourcemod/hl2sdk.h"

#ifndef PLATFORM_MAX_PATH
    #ifdef SOURCEMOD_BUILD
        #define PLATFORM_MAX_PATH MAX_PATH
    #else
        #define PLATFORM_MAX_PATH 260
    #endif
#endif

// Current compiler doesn't support parenthesis to pass multiple things as the same param for macros
#define P(...) __VA_ARGS__

#include "Libs/json/json.hpp"
using json = nlohmann::json;

#include "Common/CSingleton.h"
#include "Common/Console.h"

#include "LuaEngine.h"
using namespace luabridge;

#include "LuaScriptManager.h"
#include "LuaScriptManifest.h"
#include "LuaScript.h"
#include "LuaRuntime.h"
#include "LuaScripting.h"
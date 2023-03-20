#pragma once

#ifndef LUASCRIPTING_H
#define LUASCRIPTING_H

#include "shared.h"

namespace LuaScripting
{
    auto Initialize(void) -> void;
    auto Release(void) -> void;
}

#endif
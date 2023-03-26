#ifndef HL2SDK_H
#define HL2SDK_H

#include "hl2sdk_config.h"

#ifdef HL2SDK_ENABLE_EVENTMANAGER
#include <igameevents.h>
#endif // HL2SDK_ENABLE_EVENTMANAGER

#ifdef HL2SDK_ENABLE_EVENTMANAGER
extern IGameEventManager2* gameevents;
#endif // HL2SDK_ENABLE_EVENTMANAGER

#endif // HL2SDK_H
#pragma once

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "../shared.h"

class EventListener final : public IGameEventListener2
{

};

extern IGameEventListener2* gameevents;

#endif
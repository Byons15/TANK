#include "../Event.h"
#include "../Director.h"

EventInterface::~EventInterface()
{
}

void EventInterface::eventHookProc(const SDL_Event & event)
{
	//default
}

void EventInterface::userEventHookProc(const SDL_UserEvent & event)
{
	//default
}


#pragma once
#include <SDL_timer.h>
#include "Event.h"

class Director;

class Timer
	:public EventInterface
{
public:
	Timer();
	static Uint32 current() {
		if (onlyInstance->m_keyDown)
			return onlyInstance->m_pauseTime - onlyInstance->m_pausedTime;
		else
			return SDL_GetTicks() - onlyInstance->m_pausedTime;
	}
	static Uint32 SDLTimer() {
		return SDL_GetTicks();
	}
	~Timer();
private:
	virtual void eventHookProc(const SDL_Event & event);
	void start();
	friend class Director;
	
	static Timer *onlyInstance;
	Uint32 m_pauseTime, m_pausedTime;
	bool m_keyDown;
};


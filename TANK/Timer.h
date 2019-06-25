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
	static int addTimer(Uint32 interval, SDL_TimerCallback callbackFun, void *param) 
	{
		return SDL_AddTimer(interval, callbackFun, param);
	}
	static void removeTimer(int timerID)
	{
		SDL_RemoveTimer(timerID);
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


#include "..\Timer.h"
Timer *Timer::onlyInstance = nullptr;

Timer::Timer()
	:m_keyDown(false), m_pausedTime(0), m_pauseTime(0)
{
	if (!onlyInstance)
		onlyInstance = this;
	else
		throw std::runtime_error("Timer是单例模式");


}


Timer::~Timer()
{
}

void Timer::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_p) {
			if (!m_keyDown) {
				m_pauseTime = SDL_GetTicks();
				m_keyDown = true;
				//printf("pause current timer： %d,  SDLTime: %d, pausedTime: %d\n", current(), SDLTimer(), m_pausedTime);
			}
			else {
				m_pausedTime += SDL_GetTicks() - m_pauseTime;
				m_keyDown = false;
				//printf("Not current timer： %d,  SDLTime: %d, pausedTime: %d\n", current(), SDLTimer(), m_pausedTime);
			}
		}
			break;
	default:
		break;
	}
}

void Timer::start()
{
	setEventHook(SDL_KEYDOWN);
}

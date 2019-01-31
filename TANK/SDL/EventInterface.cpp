#include "../Event.h"
#include "../Director.h"


EventInterface::~EventInterface()
{
	//Ð¶ÔØ¹³×Ó¡£

	for (auto &iter : m_eventHook) {
		director->unsetEventHook(this, iter);
	}

	for (auto &iter : m_userEventHook) {
		director->unsetUserEventHook(this, iter);
	}
}

int EventInterface::setEventHook(int type)
{
	auto result = director->setEventHook(this, type);
	if (!result) {
		m_eventHook.insert(type);
	}
	return result;
}

void EventInterface::unsetEventHook(int type)
{
	director->unsetEventHook(this, type);
	m_eventHook.erase(type);
}

int EventInterface::setUserEventHook(int type)
{
	auto result = director->setUserEventHook(this, type);
	if (!result) {
		m_userEventHook.insert(type);
	}
	return result;
}

void EventInterface::unsetUserEventHook(int type)
{
	director->unsetUserEventHook(this, type);
	m_userEventHook.erase(type);
}

int EventInterface::userEventTrigger(const SDL_UserEvent & userEvent)
{
	return director->userEventTrigger(userEvent);
}

void EventInterface::eventHookProc(const SDL_Event & event)
{
	//default
}

void EventInterface::userEventHookProc(const SDL_UserEvent & event)
{
	//default
}
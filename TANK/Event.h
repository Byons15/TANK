#pragma once
#include <SDL_events.h>
#include <functional>
#include "Director.h"

/*
封装SDL事件以及SDL用户事件，用钩子的方式实现。
*/
class EventInterface
{
	friend Director;
public:
	virtual ~EventInterface()
	{
		uninstallEventHook();
		uninstallUserEventHook();
	}
protected:
	//see director::userEventTrigger
	void userEventTrigger(const SDL_UserEvent &userEvent)
	{
		director->userEventTrigger(userEvent);
	}

	inline void installEventHook()
	{
		director->setEventHook(this);
	}
	inline void uninstallEventHook()
	{
		director->unsetEventHook(this);
	}
	inline void installUserEventHook()
	{
		director->setUserEventHook(this);
	}
	inline void uninstallUserEventHook()
	{
		director->unsetUserEventHook(this);
	}
	
	//SDL事件钩子回调函数。
	//调用Director::setEventHook函数，在事件触发时会回调这个函数。
	//派生类实现。
	virtual void eventHookProc(const SDL_Event & event)
	{
		//DEFAULT
	}

	//SDL用户事件钩子回调函数。
	//这个是用于用户自定义事件的钩子回调，由Director::setUserEventHook设置。
	//它应该是由Director::userEventTrigger广播的事件。确切的说应该算消息。
	virtual void userEventHookProc(const SDL_UserEvent &event)
	{
		//DEFAULT
	}

	
private:
};

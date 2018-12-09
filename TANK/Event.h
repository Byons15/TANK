#pragma once
#include <SDL_events.h>

class Games;

/*
封装SDL事件以及SDL用户事件，用钩子的方式实现。
*/
class EventInterface
{
	friend Games;
public:
	virtual ~EventInterface() = default;
protected:
	
	//SDL事件钩子回调函数。
	//调用Games::setEventHook函数，在事件触发时会回调这个函数。
	//派生类实现。
	virtual void eventHookProc(const SDL_Event & event) = 0;

	//SDL用户事件钩子回调函数。
	//这个是用于用户自定义事件的钩子回调，由Games::setUserEventHook设置。
	//它应该是由Games::userEventTrigger广播的事件。确切的说应该算消息。
	virtual void userEventHookProc(const SDL_UserEvent &event) = 0;

};

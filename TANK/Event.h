#pragma once
#include <SDL_events.h>

#define EVENT_FILTER 0x901

class Games;

/*
封装SDL_UserEvent的类。
Event以SDL_UserEvent中的type成员作为事件标识。
Event不能处理SDL_UserEvent以外的事件。
*/
class Event
{
	friend Games;
public:

	enum CALLWAY
	{
		Async,
		Sync,
	};

	//触发事件。
	//callWay == Sync : 函数直接呼叫派生类实现的eventProc，将事件交其处理。 
	//callWay == Async : 函数将事件push到SDL的事件队列中，异步呼叫eventProc。
	int eventTrigger(int type, int code, void *data1 = 0, void *data2 = 0, CALLWAY callWay = Async);
	virtual ~Event() = default;
protected:
	//事件处理。
	//被拦截的事件在触发后会调用这个函数。
	//派生类需要实现它。
	virtual void eventProc(const SDL_UserEvent &event) = 0;
};

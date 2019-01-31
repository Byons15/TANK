#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>
#include "Scene.h"

class EventInterface;

class Director
{
	friend class EventInterface;
public:

	struct KeyState
	{
		SDL_Keycode key;
		Uint32 time;
		bool press;
	};

	Director();

	//delete
	Director(const Director &) = delete;
	Director(const Director &&) = delete;
	Director &operator = (const Director &) = delete;
	Director &operator = (const Director &&) = delete;

	virtual ~Director();
	
	int monitoringKey(SDL_Keycode key);
	int monitoringKey(std::initializer_list<SDL_Keycode> keyList);

	//返回按键被按下的时间点(从程序初始化开始的时间), 没有按下则返回0
	int keyState(SDL_Keycode key);

	//触发SDL_User事件。
	//要触发SDL事件请直接使用SDL API.
	//返回值参见SDL_PushEvent
	int userEventTrigger(const SDL_UserEvent &userEvent);

	int exec();
	void quit();

protected:

private:
	
	//设置SDL事件钩子，这个钩子不能拦截SDL_user事件，要拦截user事件，请用setUserEventHook。
	//EventInterface派生类重新实现eventHookProc以接收并处理事件。
	//设置成功返回0， 返回-1表示事件类型已经设置了，请直接在eventHookProc中处理。
	int setEventHook(EventInterface *event, int type);
	void unsetEventHook(EventInterface *event, int type);

	//设置SDL_user事件钩子。
	//EventInterface派生类重新实现userEventHookProc以接收并处理事件。
	//设置成功返回0， 返回-1表示事件类型已经设置了，请直接在userEventHookProc中处理。
	int setUserEventHook(EventInterface *event, int type);
	void unsetUserEventHook(EventInterface *event, int type);

	std::map<SDL_Keycode, int> m_monitoringKey;
	std::multimap<int, EventInterface *> m_eventHook;
	std::multimap<int, EventInterface *> m_userEventHook;
};

//导演
extern Director *director;

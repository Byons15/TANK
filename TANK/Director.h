#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>
#include "Scene.h"
#include <set>

class EventInterface;

class Director
{
	
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

	//返回按键被按下的时间戳, 没有按下则返回0
	int keyState(SDL_Keycode key, int *retkeyDownTime = 0);

	//触发SDL_User事件。
	//要触发SDL事件请直接使用SDL API.
	//返回值参见SDL_PushEvent
	int userEventTrigger(const SDL_UserEvent &userEvent);

	int exec();
	void quit();

protected:

private: friend class EventInterface;
	
	//设置SDL事件钩子，这个钩子不能拦截SDL_user事件，要拦截user事件，请用setUserEventHook。
	//EventInterface派生类重新实现eventHookProc以接收并处理事件。
	//设置成功返回0， 返回-1表示事件类型已经设置了，请直接在eventHookProc中处理。
	void setEventHook(EventInterface *event){m_eventHook.insert(event);}
	void unsetEventHook(EventInterface *event){m_eventHook.erase(event);}

	//设置SDL_user事件钩子。
	//EventInterface派生类重新实现userEventHookProc以接收并处理事件。
	//设置成功返回0， 返回-1表示事件类型已经设置了，请直接在userEventHookProc中处理。
	void setUserEventHook(EventInterface *event){m_userEventHook.insert(event);}
	void unsetUserEventHook(EventInterface *event){m_userEventHook.erase(event);}

private:
	std::map<SDL_Keycode, int> m_monitoringKey;
	std::set<EventInterface *> m_eventHook, m_userEventHook;
};

//导演
extern Director *director;
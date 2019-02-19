#include "..\Director.h"
#include <SDL_ttf.h>
#include "../Event.h"
#include <stdexcept>

#include <SDL_image.h>

#define USER_EVENT 0x4000

Director *director = nullptr;

Director::Director()
{
	if (director)
		throw std::runtime_error("Director类只能同时有唯一实例");
	else
		director = this;

	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();
}

Director::~Director()
{
	TTF_Quit();
	SDL_Quit();
}

int Director::monitoringKey(SDL_Keycode key)
{
	m_monitoringKey[key] = -1;
	return 0;
}

int Director::monitoringKey(std::initializer_list<SDL_Keycode> keyList)
{
	for (auto key : keyList) {
		Director::monitoringKey(key);
	}
	return 0;
}

int Director::keyState(SDL_Keycode key, int *retkeyDownTime)
{
	auto iter = m_monitoringKey.find(key);
	if (iter == m_monitoringKey.end())
		return 0;
		
	if (iter->second == -1)
		return 0;

	if (retkeyDownTime)
		*retkeyDownTime = SDL_GetTicks() - iter->second;

	return iter->second;
}

int Director::setEventHook(EventInterface * event, int type)
{
	auto &range = m_eventHook.equal_range(type);
	for (auto iter = range.first; iter != m_eventHook.end() && iter != range.second; ++iter) {
		if (event == iter->second)
			return -1;
	}

	m_eventHook.insert({type, event});
	return 0;
}

void Director::unsetEventHook(EventInterface * event, int type)
{
	auto beg = m_eventHook.lower_bound(type);
	auto end = m_eventHook.upper_bound(type);
	if (beg == m_eventHook.end())
		return;
	

	for (auto iter = beg; iter != m_eventHook.end() && iter != end; ++iter) {
		if (event == iter->second) {
			m_eventHook.erase(iter);
			break;
		}
	}
}

int Director::setUserEventHook(EventInterface * event, int type)
{
	auto &range = m_userEventHook.equal_range(type);
	for (auto iter = range.first; iter != m_userEventHook.end() && iter != range.second; ++iter) {
		if (event == iter->second)
			return -1;
	}

	m_userEventHook.insert({ type, event });
	return 0;
}

void Director::unsetUserEventHook(EventInterface * event, int type)
{
	auto &range = m_userEventHook.equal_range(type);

	if (range.first == m_userEventHook.end())
		return;

	for (auto iter = range.first; iter != m_userEventHook.end() && iter != range.second; ++iter) {
		if (event == iter->second) {
			m_userEventHook.erase(iter);
			break;
		}
	}
}

int Director::userEventTrigger(const SDL_UserEvent & user)
{
	SDL_Event event;
	event.user = user;
	event.user.type = event.type = SDL_USEREVENT;
	event.user.code = USER_EVENT;
	event.user.data1 = new SDL_UserEvent(user);
	
	return SDL_PushEvent(&event);
}

int Director::exec()
{
	SDL_Event event;
	bool quit = false;
	while (!quit) {
		if (SDL_WaitEvent(&event) != 1)
			continue;

		switch (event.type)
		{
		case SDL_USEREVENT:
		{
			switch (event.user.code)
			{
			case USER_EVENT:
			{

				//分发用户事件。
				auto user = (SDL_UserEvent *)event.user.data1;
				auto &range = m_userEventHook.equal_range(user->type);
				for (auto iter = range.first; iter != range.second && iter != m_userEventHook.end(); ++iter)
					iter->second->userEventHookProc(*user);

				delete user;
			}
			}
		}
		break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			auto iter = m_monitoringKey.find(event.key.keysym.sym);
			if (iter != m_monitoringKey.end()) {
				
				//key down
				if (event.type == SDL_KEYDOWN) {
					if (iter->second == -1) {   //只记录第一次KeyDown事件，保证成员time是第一次KeyDown事件的时间戳。
						iter->second = event.key.timestamp;
					}
				}
				//key up
				else {
					iter->second = -1;
				}
			}
		}
		//进入事件分发。

		default:
		{
			//分发SDL事件。
			auto &range = m_eventHook.equal_range(event.type);
			for (auto iter = range.first; iter != range.second && iter != m_eventHook.end(); ++iter) {
				iter->second->eventHookProc(event);
			}

			break;
		}
		}
	}

	return 0;
}

void Director::quit()
{
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}
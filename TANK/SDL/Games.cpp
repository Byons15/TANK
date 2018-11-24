#include "..\Games.h"
#include <SDL_ttf.h>
#include "../Event.h"
#include <stdexcept>

#define USER_EVENT 0x4000

Games *games = nullptr;

int main(int argc, char *argv[])
{
	Games g;
	g.exec();

	return 0;
}

Games::Games()
{
	if (games)
		throw std::runtime_error("Games类只能同时有唯一实例");
	else
		games = this;

	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();
}

Games::~Games()
{
	TTF_Quit();
	SDL_Quit();
}

int Games::monitoringKey(SDL_Keycode key)
{
	m_monitoringKey[key].key = key;
	return 0;
}

int Games::monitoringKey(std::initializer_list<SDL_Keycode> keyList)
{
	for (auto key : keyList) {
		Games::monitoringKey(key);
	}
	return 0;
}

const Games::KeyState & Games::keyState(SDL_Keycode key)
{
	auto iter = m_monitoringKey.find(key);
	if (iter == m_monitoringKey.end())
		throw std::out_of_range("没有监控这个按键");
		
	return iter->second;
}

int Games::setEventHook(Event * event, int type)
{
	auto &range = m_eventHook.equal_range(type);
	for (auto iter = range.first; iter != m_eventHook.end() && iter != range.second; ++iter) {
		if (event == iter->second)
			return -1;
	}

	m_eventHook.insert({type, event});
	return 0;
}

int Games::setUserEventHook(Event * event, int type)
{
	auto &range = m_userEventHook.equal_range(type);
	for (auto iter = range.first; iter != m_userEventHook.end() && iter != range.second; ++iter) {
		if (event == iter->second)
			return -1;
	}

	m_userEventHook.insert({ type, event });
	return 0;
}

int Games::userEventTrigger(const SDL_UserEvent & user)
{
	SDL_Event event;
	event.user = user;
	event.user.type = event.type = SDL_USEREVENT;
	event.user.code = USER_EVENT;
	event.user.data1 = new SDL_UserEvent(user);
	
	return SDL_PushEvent(&event);
}

int Games::exec()
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
				for (auto iter = range.first; iter != range.second && iter != m_userEventHook.end(); ++iter) {
					iter->second->userEventHookProc(*user);
				}
				delete user;
			}
			}
			break;
		}
		case SDL_KEYDOWN:
		{
			auto iter = m_monitoringKey.find(event.key.keysym.sym);
			if (iter != m_monitoringKey.end()) {
				if (!iter->second.press) {   //只记录第一次KeyDown事件，保证成员time是第一次KeyDown事件的时间戳。
					iter->second.press = true;
					iter->second.time = event.key.timestamp;
				}
			}
		}
			break;

		case SDL_KEYUP: 
		{
			auto iter = m_monitoringKey.find(event.key.keysym.sym);
			if (iter != m_monitoringKey.end()) {
				iter->second.press = false;
				iter->second.time = event.key.timestamp;
			}
		}
			break;

		case SDL_QUIT:
			quit = true;
			break;

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

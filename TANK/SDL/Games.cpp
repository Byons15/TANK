#include "..\Games.h"
#include <SDL_ttf.h>
#include "../Event.h"
#include <stdexcept>

static Games *onlyInstance = nullptr;

Games::Games(Uint32 initFlags)
{
	if (onlyInstance)
		throw std::runtime_error("Games类只能同时有唯一实例");
	else
		onlyInstance = this;

	SDL_Init(initFlags);
	TTF_Init();
}


Games::~Games()
{
	TTF_Quit();
	SDL_Quit();
}

int Games::monitoringKey(SDL_Keycode key)
{
	onlyInstance->m_monitoringKey[key].key = key;
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
	auto iter = onlyInstance->m_monitoringKey.find(key);
	if (iter == onlyInstance->m_monitoringKey.end())
		throw std::out_of_range("没有监控这个按键");
		
	return iter->second;
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
			case EVENT_FILTER: 
			{
				SDL_UserEvent *user = (SDL_UserEvent *)event.user.data2;
				auto e = (Event *)event.user.data1;
				e->eventProc(*user);
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
		default:
			break;
		}
	}

	return 0;
}

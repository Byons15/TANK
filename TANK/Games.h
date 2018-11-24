#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>

class Event;

class Games
{
	friend class Event;
public:

	struct KeyState
	{
		SDL_Keycode key;
		Uint32 time;
		bool press;
	};

	Games();
	virtual ~Games();
	
	int monitoringKey(SDL_Keycode key);
	int monitoringKey(std::initializer_list<SDL_Keycode> keyList);
	const KeyState &keyState(SDL_Keycode key);

	int setEventHook(Event *event, int type);
	int setUserEventHook(Event *event, int type);
	int userEventTrigger(const SDL_UserEvent &userEvent);
	int exec();

protected:

private:
	std::map<SDL_Keycode, KeyState> m_monitoringKey;
	std::multimap<int, Event *> m_eventHook;
	std::multimap<int, Event *> m_userEventHook;
};

extern Games *games;

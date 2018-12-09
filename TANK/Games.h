#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>

class EventInterface;

class Games
{
	friend class EventInterface;
public:

	struct KeyState
	{
		SDL_Keycode key;
		Uint32 time;
		bool press;
	};

	Games();

	//delete
	Games(const Games &) = delete;
	Games(const Games &&) = delete;
	Games &operator = (const Games &) = delete;
	Games &operator = (const Games &&) = delete;

	virtual ~Games();
	
	int monitoringKey(SDL_Keycode key);
	int monitoringKey(std::initializer_list<SDL_Keycode> keyList);
	const KeyState &keyState(SDL_Keycode key);

	int setEventHook(EventInterface *event, int type);
	int setUserEventHook(EventInterface *event, int type);
	int userEventTrigger(const SDL_UserEvent &userEvent);
	int exec();

protected:

private:
	std::map<SDL_Keycode, KeyState> m_monitoringKey;
	std::multimap<int, EventInterface *> m_eventHook;
	std::multimap<int, EventInterface *> m_userEventHook;
};

extern Games *games;

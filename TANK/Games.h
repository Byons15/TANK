#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>

class Games
{
public:

	struct KeyState
	{
		SDL_Keycode key;
		Uint32 time;
		bool press;
	};

	Games(Uint32 initFlags);
	virtual ~Games();
	static int monitoringKey(SDL_Keycode key);
	static int monitoringKey(std::initializer_list<SDL_Keycode> keyList);
	static const KeyState &keyState(SDL_Keycode key);
protected:
	int exec();
private:
	std::map<SDL_Keycode, KeyState> m_monitoringKey;
};


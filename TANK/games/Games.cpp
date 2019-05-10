#include "..\Games.h"
#include "../TANKS.h"

#include "AI.h"

int main(int argc, char *argv[])
{
	Games g;
	g.exec();

	return 0;
}

Games::Games()
	:m_window(SDL_CreateWindow("TANK",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)),
	m_renderer(m_window, true), m_startMenu(&m_renderer), m_ground(&m_renderer), m_P1commander(Player::P1), m_P2commander(Player::P2),
	m_stateMenu(&m_renderer)
{
	//TODO::Debug
	m_P1 = m_ground.addTank(0, Ground::ALLISE, 0);
	m_P1->setCommander(&m_P1commander);
	m_ground.maps().createMaps(1);
	m_ground.open(0, 0);
	auto t = m_ground.addTank(3, Ground::ENEMY, 0);
	t->setCommander(new AI);
	m_stateMenu.setEnemyCount(7);
	m_stateMenu.setPlayerScore(Player::P1, 5000);
	m_stateMenu.setPlayerLife(Player::P2, 3);
	m_stateMenu.setLevel(3);
	m_stateMenu.open(0, 0);


	printf("%s, %s, %s, %s, %s\n,", SDL_GetKeyName(SDLK_a), SDL_GetKeyName(SDLK_DOWN), SDL_GetKeyName(SDLK_LEFT), SDL_GetKeyName(SDLK_KP_PERIOD), SDL_GetKeyName(SDLK_KP_0));
}


Games::~Games()
{
	SDL_DestroyWindow(m_window);
}

void Games::eventHookProc(const SDL_Event & event)
{
	//TODO
}

void Games::userEventHookProc(const SDL_UserEvent & user)
{
	switch (user.type)
	{
	case StartMenu::CLOSE: {
		int model = reinterpret_cast<int>(user.data1);
		int playerNumber = reinterpret_cast<int> (user.data2);

		//TODO::
	}
		break;
	default:
		break;
	}
}

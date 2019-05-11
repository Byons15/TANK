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
	m_stateMenu(&m_renderer), m_customMap(&m_renderer, &m_ground)
{
	EventInterface::setUserEventHook(StartMenu::CLOSE);
	m_startMenu.open(0, 0);
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
		if (model == 1) {
			//TODO::
		}
		else if(model == 2){
			m_ground.maps().clearMap();
			m_ground.open(0, 0);
			m_customMap.open(0, 0);
		}
	}
		break;
	default:
		break;
	}
}

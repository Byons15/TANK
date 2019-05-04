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
	 m_renderer(m_window, true), m_startMenu(&m_renderer), m_ground(&m_renderer), m_P1commander(Player::P1), m_P2commander(Player::P2)
{
	//TODO::Debug
	m_P1 = m_ground.addTank(0, Ground::ALLISE, 0);
	m_P1->setCommander(&m_P1commander);
	m_ground.maps().createMaps(1);
	m_ground.open(0, 0);
	auto t = m_ground.addTank(3, Ground::ENEMY, 0);
	t->setCommander(new AI);
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
	case StartMenu::CLOSE:
		//TODO::
		break;
	default:
		break;
	}
}

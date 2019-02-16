#include "..\Games.h"
#include "../TANKS.h"

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
	 m_renderer(m_window, true), m_startMenu(&m_renderer), m_ground(&m_renderer), m_normal(&m_ground)
{
	//TODO:: debug
	//m_ground.open(0, 0);
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

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
	 m_renderer(m_window, false), m_startMenu(&m_renderer)
{
	//TODO::debug
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
	case StartMenu::CLOSE:
		//TODO::Debug.
		printf("%d, %d", user.data1, user.data2);
		getchar();
		break;
	default:
		break;
	}
}

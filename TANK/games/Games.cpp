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
	m_renderer(m_window, true), m_startMenu(&m_renderer), m_ground(&m_renderer),
	m_stateMenu(&m_renderer), m_customMap(&m_renderer, &m_ground), m_resultView(&m_renderer)
{
	EventInterface::setUserEventHook(StartMenu::CLOSE);
	//m_startMenu.open(0, 0);

	ResultView::RESULT r;
	r.lists[0][0] = 5;
	r.lists[0][1] = 500;
	r.lists[1][0] = 3;
	r.lists[1][1] = 300;
	r.lists[2][0] = 2;
	r.lists[2][1] = 600;
	r.lists[3][0] = 3;
	r.lists[3][1] = 900;
	m_resultView.open(reinterpret_cast<void *>(&r), 1);
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
		m_gameModel = reinterpret_cast<int>(user.data1);
		m_playerNumber = reinterpret_cast<int> (user.data2);
		m_ground.open(0, 0);
		m_stateMenu.open(0, 0);

		if (m_gameModel == 1) {
			
			//设置状态栏
			m_stateMenu.setEnemyCount(17);
			m_stateMenu.setLevel(1);
			m_stateMenu.setPlayerLife(Player::P1, 4);

			//布置战场。
			m_ground.maps().createMaps(1);
			m_P1 = m_ground.addTank(Tank::P1, Ground::ALLISE, 0);
			m_P1->setCommander(new Player(Player::P1));
			if (m_playerNumber == 2) {
				m_P2 = m_ground.addTank(Tank::P2, Ground::ALLISE, 1);
				m_P2->setCommander(new Player(Player::P2));
				m_stateMenu.setPlayerLife(Player::P2, 4);
			}
		}
		else {
			m_ground.maps().clearMap();
			m_customMap.open(0, 0);
		}
	}
		break;
	case CustomMap::CLOSE:
		//设置状态栏
		m_stateMenu.setEnemyCount(17);
		m_stateMenu.setLevel(1);
		m_stateMenu.setPlayerLife(Player::P1, 4);

		//布置战场。
		m_ground.maps().createMaps(1);
		m_P1 = m_ground.addTank(Tank::P1, Ground::ALLISE, 0);
		m_P1->setCommander(new Player(Player::P1));
		if (m_playerNumber == 2) {
			m_P2 = m_ground.addTank(Tank::P2, Ground::ALLISE, 1);
			m_P2->setCommander(new Player(Player::P2));
			m_stateMenu.setPlayerLife(Player::P2, 4);
		}
		break;
	default:
		break;
	}
}

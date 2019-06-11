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
	installEventHook();
	installUserEventHook();
	m_startMenu.show(0, 0);
}


Games::~Games()
{
	uninstallEventHook();
	uninstallUserEventHook();
	SDL_DestroyWindow(m_window);
}

void Games::startGame(int palyerCount, int level)
{
	m_ground.show(0, 0);
	m_stateMenu.show(0, 0);

	//设置状态栏
	m_stateMenu.setEnemyCount(17);
	m_stateMenu.setLevel((level) ? level : 1);

	//布置战场。
	if(level)
		m_ground.maps().createMaps(level);

	for (size_t p = 0; p != m_playerNumber; ++p) {
		m_player[p].tank = m_ground.addTank((Tank::MODEL)p, Ground::ALLISE, p);
		m_player[p].tank->setCommander(new Player((Player::PLAYER)p));
		m_stateMenu.setPlayerLife((Player::PLAYER)p, m_player[p].life);
	}

	for (size_t i = 0; i != 3; ++i) {
		auto t = m_ground.addTank(Tank::ARMOURED1, Ground::ENEMY, i);
		t->setCommander(new AI(t, 1));
	}
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

		if (m_gameModel == 1) {
			startGame(m_playerNumber, 1);
		}
		else {
			m_ground.maps().clearMap();
			m_customMap.show(0, 0);
		}
	}
		break;
	case CustomMap::CLOSE:
		startGame(m_playerNumber, 0);

		break;
	case ResultView::CLOSE:
		if (!user.code) {
			m_startMenu.show(0, 0);
		}
		else {
			startGame(m_playerNumber, m_stateMenu.level() + 1);
		}
	case Ground::DESTORYTANK:
		
		break;
	default:
		break;
	}
}

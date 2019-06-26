#include "..\Games.h"
#include "../TANKS.h"
#include "../Timer.h"
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
		m_player[p].tank = m_ground.addTank((Tank::MODEL)p, Tank::ALLISE, p);
		m_player[p].tank->setCommander(new Player((Player::PLAYER)p));
		m_stateMenu.setPlayerLife((Player::PLAYER)p, m_player[p].life);
	}

	for (size_t i = 0; i != 3; ++i) {
		auto t = m_ground.addTank(Tank::ARMOURED1, Tank::ENEMY, i);
		t->setCommander(new AI(t, 1));
	}
}

int Games::createEnemy()
{
	if (m_stateMenu.enemyCount() == 0) {
		return -2;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> uidModel(Tank::AGILITY, Tank::ORDINARY2), uidBindPoint(0, 2);
	auto t = m_ground.addTank((Tank::MODEL)uidModel(gen), Tank::ENEMY, uidBindPoint(gen));

	if (t == 0) {
		m_EnemyCreateTimerID = Timer::addTimer(1000, enemyCreateTimeCallback, this);
		return -1;
	}

	t->setCommander(new AI(t, m_stateMenu.level()));

	return 0;
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
	case Tank::KILLED:

		//敌军阵亡.
		if (user.code == Ground::ENEMY) {  

			//更新敌军数量。
			m_stateMenu.setEnemyCount((m_stateMenu.enemyCount() - 1 > 0) ? m_stateMenu.enemyCount() - 1 : 0);

			//检查敌军数量是否为0
			if (m_stateMenu.enemyCount() == 0) {

				//触发游戏胜利事件。
				SDL_UserEvent user;
				user.type = GAME_OVER;
				user.code = 1;
				EventInterface::userEventTrigger(user);
				break;   //跳过敌军生成，因为敌军已经被消灭完了。
			}

			//1秒后再生成敌军。
			m_EnemyCreateTimerID = Timer::addTimer(1000, enemyCreateTimeCallback, this);
		}

		//友军阵亡.
		else {
			auto scorecard = reinterpret_cast<std::map<Tank::MODEL, SCORECARD> *>(user.data2);
			printf("英勇的 %d号 烈士击杀了: \n");
			for (auto &s : *scorecard)
			{
				printf("Model: %d count: %d  total: %d\n", s.first, s.second.kill, s.second.score);
			}
		}
		break;

	case CREATE_ENEMY:
		createEnemy();
		break;
	case GAME_OVER:

		break;
	default:
		break;
	}
}

Uint32 Games::enemyCreateTimeCallback(Uint32 interval, void * param)
{
	Games *g = reinterpret_cast<Games *> (param);

	SDL_UserEvent user;
	user.type = CREATE_ENEMY;
	g->EventInterface::userEventTrigger(user);

	return 0;
}

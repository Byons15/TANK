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
	m_stateMenu(&m_renderer), m_customMap(&m_renderer, &m_ground), m_resultView(&m_renderer), m_players{ Player::P1, Player::P2 }
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

	m_remainEenemy = 20;

	//设置状态栏
	m_stateMenu.setEnemyCount(m_remainEenemy);
	m_stateMenu.setLevel((level) ? level : 1);

	//布置战场。
	if(level)
		m_ground.maps().createMaps(level);

	//生成玩家.
	for (size_t p = 0; p != m_playerNumber; ++p) {
		m_playersData[p].tank = m_ground.addTank((Tank::MODEL)p, Tank::ALLISE, p);
		m_playersData[p].tank->setDriver(&m_players[p]);
		m_stateMenu.setPlayerLife((Player::PLAYER)p, m_playersData[p].life);
	}

	//生成AI
	m_inUseAICount = 0;
	for (size_t i = 0; i != 3; ++i) {
		m_ground.addTank(Tank::ORDINARY1, Tank::ENEMY, i)->setDriver(&m_AIs[m_inUseAICount++]);
	}

	m_stateMenu.setEnemyCount(m_stateMenu.enemyCount() - 3);
}

int Games::createEnemy(AI *driver)
{
	if (m_stateMenu.enemyCount() == 0) {
		return -2;
	}

	std::default_random_engine rd(Timer::current() + unsigned(driver) % (unsigned(driver) / 2));
	std::uniform_int_distribution<> uidModel(Tank::AGILITY, Tank::ORDINARY2), uidBindPoint(0, 2);
	auto t = m_ground.addTank((Tank::MODEL)uidModel(rd), Tank::ENEMY, uidBindPoint(rd));

	if (t == 0) {
		SDL_UserEvent *user = new SDL_UserEvent;
		user->type = CREATE_ENEMY;
		user->data1 = driver;
		m_EnemyCreateTimerID = Timer::addTimer(1000, enemyCreateTimeCallback, user);
		return -1;
	}

	//更新敌军数量。
	m_stateMenu.setEnemyCount(m_stateMenu.enemyCount() - 1);
	t->setDriver(driver);

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
			//检查敌军数量是否为0
			if (m_remainEenemy == 0) {

				//触发游戏胜利事件。
				SDL_UserEvent user;
				user.type = GAME_OVER;
				user.code = 1;
				EventInterface::userEventTrigger(user);
				break;   //跳过敌军生成，因为敌军已经被消灭完了。
			}

			//1秒后再生成敌军。
			SDL_UserEvent *param = new SDL_UserEvent;
			*param = user;
			param->type = CREATE_ENEMY;
			param->data2 = this;
			m_EnemyCreateTimerID = Timer::addTimer(1000, enemyCreateTimeCallback, param);
			m_remainEenemy--;
		}

		//友军阵亡.
		else {
			int model = user.timestamp;
			if (m_playersData[model].life != 0) {
				m_ground.addTank((Tank::MODEL)model, Tank::ALLISE, model)->setDriver(&m_players[model]);
				--m_playersData[model].life;
				m_stateMenu.setPlayerLife((Player::PLAYER)model, m_playersData[model].life);
			}
		}
		break;

	case CREATE_ENEMY:
		createEnemy(((AI *)(user.data2)));
		break;
	case GAME_OVER:
		if (user.code == 1) {
			printf("winner player!!\n");
			getchar();
		}

		break;
	default:
		break;
	}
}

Uint32 Games::enemyCreateTimeCallback(Uint32 interval, void * param)
{
	director->userEventTrigger(*(reinterpret_cast<SDL_UserEvent *> (param)));
	delete ((SDL_UserEvent *)param);
	return 0;
}

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
		m_playersData[p].life--;
		m_stateMenu.setPlayerLife((Player::PLAYER)p, m_playersData[p].life);
		m_players[p].clearScorecards();
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
	std::uniform_int_distribution<> uidModel(Tank::ORDINARY1, Tank::ARMOURED3), uidBindPoint(0, 2);
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

		for (int i = 0; i != m_playerNumber; ++i) {
			m_playersData[i].life = 5;
		}

		if (m_gameModel == 1) {
			startGame(m_playerNumber, 1);
		}
		else {
			m_ground.show(0, 0);
			m_stateMenu.show(0, 0);
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
			for (auto i = 0; i != m_playerNumber; ++i) {
				m_playersData[i].life++;
			}
			startGame(m_playerNumber, m_stateMenu.level() + 1);
		}
		break;

	case Tank::KILLED:

		//敌军阵亡.
		if (user.code == Ground::ENEMY) {  

			//检查敌军数量是否为0
			if (--m_remainEenemy == 0) {

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
			m_EnemyCreateTimerID = Timer::addTimer(1000, enemyCreateTimeCallback, param);
			if (user.data1 == &m_players[0]) {
				m_stateMenu.setPlayerScore(Player::P1, m_stateMenu.playerScore(Player::P1) + Driver::queryScoreForModel((Tank::MODEL)user.timestamp));
			}
			else if (user.data1 == &m_players[1]) {
				m_stateMenu.setPlayerScore(Player::P2, m_stateMenu.playerScore(Player::P2) + Driver::queryScoreForModel((Tank::MODEL)user.timestamp));
			}
		}

		//友军阵亡.
		else {
			int model = user.timestamp;
			if (m_playersData[model].life != 0) {
				m_ground.addTank((Tank::MODEL)model, Tank::ALLISE, model)->setDriver(&m_players[model]);
				--m_playersData[model].life;
				m_stateMenu.setPlayerLife((Player::PLAYER)model, m_playersData[model].life);
			}
			else {
				if (!m_playersData[0].life && !m_playersData[1].life) {
					SDL_UserEvent user;
					user.type = GAME_OVER;
					user.code = 0;
					EventInterface::userEventTrigger(user);
				}
			}
		}
		break;

	case CREATE_ENEMY:
		createEnemy(((AI *)(user.data2)));
		break;
	case GAME_OVER:
		m_ground.hide();
		m_stateMenu.hide();

		if (user.code == 1) {
			SCORECARDS *result[2] = {&m_players[0].scoreCards(), &m_players[1].scoreCards()};
			m_resultView.show(&result, m_playerNumber);
		}
		else {
			m_startMenu.show(0, 0);
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

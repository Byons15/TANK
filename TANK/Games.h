#pragma once
#include "Director.h"
#include "Renderer.h"
#include "StartMenu.h"
#include "Event.h"
#include "ground\Ground.h"
#include "games\Player.h"
#include "StateMenu.h"
#include "games/CustomMap.h"
#include "AI.h"
#include "ResultView.h"

class Games :
	public Director,
	public EventInterface
{
public:
	Games();
	~Games();

	//level: 要开启的关卡。0表示不设置关卡（这意味着之前已经运行过自定义地图环节了）。
	void startGame(int palyerCount, int level);
	int createEnemy();

protected:
	virtual void eventHookProc(const SDL_Event &event) override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:

	enum EVENT
	{
		CREATE_ENEMY = 0x6513331,
		GAME_OVER,
	};

	static Uint32 enemyCreateTimeCallback(Uint32 interval, void *param);

	SDL_Window *m_window;
	Renderer m_renderer;
	StartMenu m_startMenu;
	StateMenu m_stateMenu;
	Ground m_ground;
	CustomMap m_customMap;
	ResultView m_resultView;
	int m_playerNumber, m_gameModel;
	Uint32 m_EnemyCreateTimerID;
	
	struct PLAYERSTATE 
	{
		Tank *tank;
		std::map<Tank::MODEL, std::pair<unsigned, unsigned>> kill;
		unsigned life;
	} m_player[2];
};


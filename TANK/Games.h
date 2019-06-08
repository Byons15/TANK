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

	//level: Ҫ�����Ĺؿ���0��ʾ�����ùؿ�������ζ��֮ǰ�Ѿ����й��Զ����ͼ�����ˣ���
	void startGame(int palyerCount, int level);

protected:
	virtual void eventHookProc(const SDL_Event &event) override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:
	SDL_Window *m_window;
	Renderer m_renderer;
	StartMenu m_startMenu;
	StateMenu m_stateMenu;
	Ground m_ground;
	CustomMap m_customMap;
	ResultView m_resultView;
	int m_playerNumber, m_gameModel;
	
	struct PLAYERSTATE 
	{
		Tank *tank;
		std::map<Tank::MODEL, std::pair<unsigned, unsigned>> kill;
		unsigned life;
	} m_player[2];
};


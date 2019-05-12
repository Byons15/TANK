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

class Games :
	public Director,
	public EventInterface
{
public:
	Games();
	~Games();

protected:
	virtual void eventHookProc(const SDL_Event &event) override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:
	SDL_Window *m_window;
	Renderer m_renderer;
	StartMenu m_startMenu;
	StateMenu m_stateMenu;
	Ground m_ground;
	Tank *m_P1, *m_P2;
	CustomMap m_customMap;
	int m_playerNumber, m_gameModel;
};


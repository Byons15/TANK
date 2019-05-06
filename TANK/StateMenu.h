#pragma once
#include "Scene.h"
#include "Spirit.h"
#include <array>
#include "Text.h"

class StateMenu :
	public Scene
{
public:
	StateMenu();
	virtual ~StateMenu();
private:
	std::array<std::pair<SDL_Point, Spirit>, 6> m_tag;
	Text P1Score, P2Score£¬P1LifeCount, P2LifeCount;
};


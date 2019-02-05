#pragma once
#include "..\Spirit.h"
class Tank :
	public Spirit
{
public:
	Tank();
	~Tank();
private:
	Animation m_rewardsStatus;
	SDL_Rect m_rect;
	int m_HP;
};


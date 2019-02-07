#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "../Ground.h"

class Tank :
	public Spirit
{
public:
	Tank(Ground * ground, int &model);
	~Tank();
	int HP() const {
		return m_HP;
	}
	int beHit(int power);
	static void setFactory(TankFactory *factory);
private:
	static TankFactory *sm_factory;
	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	SDL_Rect m_rect;
	int m_HP, m_speeds;
	Ground *m_ground;
};


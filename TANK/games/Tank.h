#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "../Ground.h"

class Tank :
	public Spirit
{
public:

	enum EVENT
	{
		BONUSCHEST = 0x300,
	};

	enum REWARDS
	{
		DEFAULT = 0,
		ADDHP,
		SUPERPOWER,
		SUPERSPEEDS,
		INVINCIBLE,
	};

	Tank(Ground * ground, int &model, REWARDS rewards);
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
	SDL_Rect m_pixelRect;
	int m_HP, m_speeds;
	Ground *m_ground;
	REWARDS m_rewarde;
};
 

#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Ground.h"
#include "Mover.h"

class Tank :
	public Spirit
{
public:
	static constexpr int colSize = GRID_SIZE * 2 - 1;
	enum EVENT
	{
		BONUSCHEST = 0x300,
	};

	enum REWARDS
	{
		DEFAULT = 0,
		ADDLIVE,
		SUPERPOWER,
		INVINCIBLE,
	};
	
	int HP() const {
		return m_HP;
	}
	inline const SDL_Point &position() const {
		return m_position;
	}
	int setPosition(const SDL_Point &pos);
	int startMove(Mover::DIRECTION direction);
	void stopMove();
	void setRewards(REWARDS r);

private: friend class Ground;
	Tank(Ground * ground, int &model, const SDL_Point &position);
	~Tank();
	int beHit(Tank *aggressor, int power);
	static void setFactory(TankFactory *factory);
	void update(Uint32 time);

private:
	static TankFactory *sm_factory;
	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	SDL_Point m_position;
	Mover m_mover;
	int m_HP, m_speeds;
	REWARDS m_rewarde;
	Ground *m_ground;
	bool m_input;
};
 

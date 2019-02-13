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
	
	int HP() const {
		return m_HP;
	}
	inline const SDL_Point &position() const {
		return m_position;
	}
	int setPosition(const SDL_Point &pos);
	int startMove(Mover::DIRECTION direction);
	void stopMove();

	//设置奖励箱
	//rewarde是用户定义的参数，这个参数会在坦克触发奖励箱时传到事件队列，一般用于标识奖励类型，传入0则取消奖励箱。
	//成功返回0， 坦克处在无敌状态则返回-1
	int setRewards(int rewarde);

	//成功返回0， 坦克当前携带有奖励箱则返回-1
	int invincible();
	void unInvincible();

private: friend class Ground;
	Tank(Ground * ground, int &model, const SDL_Point &position);
	~Tank();
	int beHit(Tank *aggressor, int power);
	static void setFactory(TankFactory *factory);
	void update(Uint32 time);

private:
	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	Mover m_mover;
	SDL_Point m_position;
	int m_HP, m_speeds, m_rewarde;
	Ground *m_ground;
	static TankFactory *sm_factory;
	bool m_input, m_invincible;
};
 

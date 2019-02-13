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

	//���ý�����
	//rewarde���û�����Ĳ����������������̹�˴���������ʱ�����¼����У�һ�����ڱ�ʶ�������ͣ�����0��ȡ�������䡣
	//�ɹ�����0�� ̹�˴����޵�״̬�򷵻�-1
	int setRewards(int rewarde);

	//�ɹ�����0�� ̹�˵�ǰЯ���н������򷵻�-1
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
 

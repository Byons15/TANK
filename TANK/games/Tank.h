#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Ground.h"
#include "Mover.h"

class Commander;

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

	void fire();

	//��ʼ�ƶ���
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove��ʹ̹�˽��롰�����ƶ���״̬�������ƶ�״̬��ʾ̹�������ƶ����������һ������ͬʱ�������ƶ�������
	void stopMove(Uint32 time);
	int setCommander(Commander *cmder);
	bool moveState() const;
	int model() const {
		return m_model;
	}

	//���ý�����
	//rewarde���û�����Ĳ����������������̹�˴���������ʱ�����¼����У�һ�����ڱ�ʶ�������ͣ�����0��ȡ�������䡣
	//�ɹ�����0�� ̹�˴����޵�״̬�򷵻�-1
	int setRewards(int rewarde);

	//�ɹ�����0�� ̹�˵�ǰЯ���н������򷵻�-1
	int invincible();

private: friend class Ground;
	Tank(Ground * ground, int &model, const SDL_Point &position);
	~Tank();
	int beHit(Tank *aggressor, int power);
	static void setFactory(TankFactory *factory);
	void update(Uint32 time);
	inline void render();

private:
	bool onGrid();
	void unInvincible();

	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	Mover m_mover;
	SDL_Point m_position;
	int m_HP, m_rewarde, m_model, m_power;
	float m_speeds, m_defaultSpeeds;
	Mover::DIRECTION m_direction;
	Ground *m_ground;
	Commander *m_commander;
	static TankFactory *sm_factory;
	bool m_stopMoving, m_invincible;
};
 
class Commander
{
public:
	Commander() = default;

	//������ʵ�ִ˺����Խ������̹�ˣ������������Tank��Ҫ�����ʱ����á�
	//fire �������෵���Ƿ񿪻�
	//direction�������෵���µ��ƶ�����
	//����ֵ����ǰ���´��ƶ�����ʱ����-1�� ���򷵻�0��
	virtual int command(Ground *ground, Tank *tank, Uint32 timestamp, bool &fire, Mover::DIRECTION &direction) = 0;
	virtual ~Commander() = default;
private:
};

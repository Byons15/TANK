#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Mover.h"
#include "../TANKS.h"

class Commander;
class Ground;

class Tank :
	public Spirit
{
public:
	static constexpr int colSize = GRID_SIZE * 2 - 1;
	enum EVENT
	{
		BONUSCHEST = 0x300,
	};

	enum MODEL
	{
		P1,
		P2,
		AGILITY,
		ARMOURED1,
		ARMOURED2,
		ARMOURED3,
		ORDINARY1,
		ORDINARY2,
	};
	
	int HP() const {
		return m_HP;
	}
	inline const SDL_Point &pixelPosition() const {
		return m_position;
	}
	inline SDL_Point position() const {
		return SDL_Point{m_position.x / GRID_SIZE, m_position.y / GRID_SIZE};
	}

	//����̹��λ��ս��������λ�á�
	int setGroundPosition(const SDL_Point &pos);   
	Mover::DIRECTION direction() const {
		return m_direction;
	}
	void fire();
	void reload();

	//��ʼ�ƶ���
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove��ʹ̹�˽��롰�����ƶ���״̬�������ƶ�״̬��ʾ̹�������ƶ����������һ������ͬʱ�������ƶ�������
	void stopMove();
	int setCommander(Commander *cmder);
	bool moveState() const {
		return m_mover.state();
	}
	MODEL model() const {
		return m_model;
	}
	Ground *ground() {
		return m_ground;
	}

	//���ý�����
	//rewarde���û�����Ĳ����������������̹�˴���������ʱ�����¼����У�һ�����ڱ�ʶ�������ͣ�����0��ȡ�������䡣
	//�ɹ�����0�� ̹�˴����޵�״̬�򷵻�-1
	int setRewards(int rewarde);

	//�ɹ�����0�� ̹�˵�ǰЯ���н������򷵻�-1
	int invincible();

private: friend class Ground;
	Tank(Ground * ground, MODEL &model, const SDL_Point &position);
	~Tank();
	int beHit(Tank *aggressor, int power);
	static void setFactory(TankFactory *factory);
	void update(Uint32 time);
	void render();

private:
	bool onGrid();
	void unInvincible();

	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	Mover m_mover;
	SDL_Point m_position;
	SDL_Rect m_lastRect;
	MODEL m_model;
	int m_HP, m_rewarde, m_power, m_killScore, m_score;
	float m_speeds, m_defaultSpeeds;
	Mover::DIRECTION m_direction;
	Ground *m_ground;
	Commander *m_commander;
	static TankFactory *sm_factory;
	bool m_invincible, m_reload;
};
 
class Commander
{
public:
	Commander();

	//������ʵ�ִ˺����Խ������̹�ˣ������������Tank��Ҫ�����ʱ����á�
	//direction�������෵���µ��ƶ�����
	//����ֵ����ǰ���´��ƶ�����ʱ����-1���´����ƶ�����ʱ����0.
	int drive(Tank *tank);
	virtual bool requestFire() = 0;
	Tank *tank() {
		return m_tank;
	}
	virtual int command(SDL_Point cuttentPosition, Uint32 timestamp, Mover::DIRECTION &direction) = 0;
	virtual ~Commander() = default;
private:
	Tank *m_tank;
};

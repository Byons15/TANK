#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Ground.h"
#include "Mover.h"
#include "../Event.h"

class Commander;

class Tank :
	public Spirit,
	public EventInterface
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

	//��ʼ�ƶ���
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove��ʹ̹�˽��롰�����ƶ���״̬�������ƶ�״̬��ʾ̹�������ƶ����������һ������ͬʱ�������ƶ�������
	void stopMove();
	int setCommander(Commander *cmder);
	bool moveState() const {
		return m_mover.state();
	}
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
	void render();

private:
	bool onGrid();
	void unInvincible();
	virtual void userEventHookProc(const SDL_UserEvent &event) override;

	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	Mover m_mover;
	SDL_Point m_position;
	SDL_Rect m_lastRect;
	int m_HP, m_rewarde, m_model, m_power;
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
	Commander() = default;

	//������ʵ�ִ˺����Խ������̹�ˣ������������Tank��Ҫ�����ʱ����á�
	//direction�������෵���µ��ƶ�����
	//����ֵ����ǰ���´��ƶ�����ʱ����-1���´����ƶ�����ʱ����0.
	virtual int command(Ground *ground, Tank *tank, SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) = 0;
	virtual ~Commander() = default;
private:
};

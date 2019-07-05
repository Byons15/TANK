#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Mover.h"
#include "../TANKS.h"

class Driver;
class Ground;

class Tank :
	public Spirit
{
public:
	static constexpr int colSize = GRID_SIZE * 2 - 1;
	enum EVENT
	{
		//�����䴥����
		//code����Ʒ
		//data1�������� type : Driver
		//data2���������ߣ�Я���������tank type : Tank
		BONUSCHEST = 0x300,

		//̹�˱����У���һ���ݻ��ˣ���
		//data1�������� type : Driver
		//data2���������ߣ�ʹ�����ָ��ǰ��ȷ��̹�˻�û�б����٣�type : Tank
		ATTACK,

		//̹�˱����٣�����¼�һ���Ǹ���Tank::ATTACKTANK�¼��ġ�
		//code�����ݻٵ�̹�˵���Ӫ��
		//data1: ������ type : Driver
		//data2���������ߡ�
		//timestamp : ��������̹���ͺš�
		KILLED,
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

	enum CAMP
	{
		ALLISE,
		ENEMY,
	};
	
	int HP() const {return m_HP;}
	inline const SDL_Point &pixelPosition() const {return m_position;}
	inline SDL_Point position() const {return SDL_Point{m_position.x / GRID_SIZE, m_position.y / GRID_SIZE};}

	//����̹��λ��ս��������λ�á�
	int setGroundPosition(const SDL_Point &pos);   
	Mover::DIRECTION direction() const {return m_direction;}
	void fire();
	void reload();

	//��ʼ�ƶ���
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove��ʹ̹�˽��롰�����ƶ���״̬�������ƶ�״̬��ʾ̹�������ƶ����������һ������ͬʱ�������ƶ�������
	void stopMove();
	int setDriver(Driver *driver);
	bool moveState() const {return m_mover.state();}
	MODEL model() const {return m_model;}
	Ground *ground() {return m_ground;}
	Driver *driver() { return m_driver; }
	CAMP camp() const { return m_camp; }

	//���ý�����
	//rewarde���û�����Ĳ����������������̹�˴���������ʱ�����¼����У�һ�����ڱ�ʶ�������ͣ�����0��ȡ�������䡣
	//�ɹ�����0�� ̹�˴����޵�״̬�򷵻�-1
	int setRewards(int rewarde);

	//�ɹ�����0�� ̹�˵�ǰЯ���н������򷵻�-1
	int invincible();

	static int attack(Driver *aggressor, Tank *target, int power);

private: friend class Ground;
	Tank(Ground * ground, CAMP camp, MODEL &model, const SDL_Point &position);
	~Tank();

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
	CAMP m_camp;
	MODEL m_model;
	int m_HP, m_rewarde, m_power;
	float m_speeds, m_defaultSpeeds;
	Mover::DIRECTION m_direction;
	Ground *m_ground;
	Driver *m_driver;
	static TankFactory *sm_factory;
	bool m_invincible, m_reload;
};
 
struct SOCRECARD
{
	int killCount = 0;
	unsigned total = 0;
};
typedef std::map<Tank::MODEL, SOCRECARD> SCORECARDS;

class Driver
{
public:
	Driver();

	//������ʵ�ִ˺����Խ������̹�ˣ������������Tank��Ҫ�����ʱ����á�
	//direction�������෵���µ��ƶ�����
	//����ֵ����ǰ���´��ƶ�����ʱ����-1���´����ƶ�����ʱ����0.
	void drive(Tank *tank) { m_tank = tank; }
	virtual bool requestFire() = 0;
	Tank *tank() { return m_tank; }
	void record(Tank::MODEL model);
	unsigned total() { return m_total; }
	void clearScorecards() { m_socrecards.clear(); }
	virtual int command(SDL_Point cuttentPosition, Uint32 timestamp, Mover::DIRECTION &direction) = 0;
	virtual ~Driver() = default;
private:
	Tank *m_tank;
	std::vector<unsigned> sm_scoreTable;
	SCORECARDS m_socrecards;
	unsigned m_total;
};

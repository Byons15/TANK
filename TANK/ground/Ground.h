#pragma once
#include "../Scene.h"
#include "Maps.h"
#include "../Event.h"
#include <map>
#include <set>
#include "TankFactory.h"
#include <functional>
#include "Missile.h"
#include "Tank.h"
#include "../games/Navigator.h"

class Tank;

class Ground :
	public Scene,
	public EventInterface
{
public:

	enum CAMP
	{
		ALLISE,
		ENEMY,
	};

	enum EVENT
	{
		//̹�˱����٣�����¼�һ���Ǹ���Tank::ATTACKTANK�¼��ġ�
		//code�����ݻٵ�̹�˵���Ӫ��
		//data1�����ݻٵ�̹���ͺš�
		//data2: ���ݻٵ�̹�˵ļƷְ�..
		DESTORYTANK = 0x999,
	};

	Ground(Renderer *renderer);
	~Ground();

	Maps &maps() {return m_maps;}
	const ROAD &colMap() const {return m_colMap;}

	const TankFactory &tankFactory() {return m_tankFactory;}
	
	//�ɹ�����ָ�룬����0��ʾ�������������̹�ˣ���ռ���ˣ�����ʧ�ܡ� 
	Tank* addTank(Tank::MODEL tankModel, Tank::CAMP camp, int bindIndex);

	void addMissile(Missile *m);

	//����̹�ˡ�
	//̹���Ѵݻٷ���0�� ̹�������˷���1�� ̹�˴����޵�״̬����-1
	int attackTank(Driver * aggressor, Tank *tank, int power);

	int attackTerrain(const SDL_Point &pos, int power);

	//�ݻ�̹�ˣ� �����ǿ�Ƶ�
	void destoryTank(Tank * target);

	void destoryBase();

	void destoryTerrain(const SDL_Point &pos);

	const std::set<Tank *> &tankList() const { return m_tanks; }

	//̹����ײ���
	//û����ײ����0�� ����η�����ײ����-1��������̹�˷�����ײ�򷵻�-2,������ײ̹�˸�ֵretColDest
	int positionTest(Tank *tank, const SDL_Point &pixelPos, Tank ** retColDest);

	int findTankOnRect(Tank *tank, const SDL_Rect &rect, std::vector<Tank *>& outResult);

	//�ڵ���ײ���
	//û����ײ����0�� ����η�����ײ����-1����̹�˷�����ײ�򷵻�-2, �߽���ײ����-3
	int MissilepositionUpdate(Missile *m);

	//Ͷ��ȡ��֮��������maxX��maxY��ɵľ��󣬽�ÿ��λ�ûص�p
	static void foreachRect(int maxX, int maxY, std::function<void(int x, int y)> p);

	//������ײӳ��
	void updateColMap(int x, int y);

protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;
	virtual void open(void *data, int code) override;
	virtual void close() override;

private:
	void clearGround();

	Maps m_maps;

	TankFactory m_tankFactory;
	std::set<Tank *> m_tanks;
	ROAD m_colMap;
	std::array<std::array<Tank *, MAP_SIZE>, MAP_SIZE> m_tanksMap;

	std::list<Missile *> m_missiles;
};


SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect);
SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint);
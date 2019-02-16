#pragma once
#include "../Scene.h"
#include "Maps.h"
#include "../Event.h"
#include <map>
#include <set>
#include "TankFactory.h"
#include <functional>
#include "Missile.h"

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
		DESTORYTANK = 0x999,
	};

	Ground(Renderer *renderer);
	~Ground();

	virtual void open(void *data, int code) override;
	virtual void close() override;

	//����ͼ
	Maps &maps() {
		return m_maps;
	}
	const TankFactory &tankFactory() {
		return m_tankFactory;
	}

	//�ɹ�����ָ�룬����0��ʾ�������������̹�ˣ���ռ���ˣ�����ʧ�ܡ� 
	Tank* addTank(int tankModel, CAMP camp, int bindIndex);

	void addMissile(Missile *m);

	//����̹�ˡ�
	//̹���Ѵݻٷ���0�� ̹�������˷���1�� ̹�˴����޵�״̬����-1
	int attackTank(Tank *tank, int power);

	int attackTerrain(const SDL_Point &pos, int power);

	//�ݻ�̹�ˣ� �����ǿ�Ƶ�
	void destoryTank(Tank *tank);

	void destoryBase();

	void destoryTerrain(const SDL_Point &pos);

	//̹����ײ���
	//û����ײ����0�� ����η�����ײ����-1��������̹�˷�����ײ�򷵻�-2,������ײ̹�˸�ֵretColDest
	int tankColCheck(Tank *tank, const SDL_Point &pixelPos, Tank ** retColDest);

	//�ڵ���ײ���
	//û����ײ����0�� ����η�����ײ����-1����̹�˷�����ײ�򷵻�-2, �߽���ײ����-3
	int missileColCheck(Missile *m, const SDL_Point &checkPos, Tank ** retColDest, SDL_Point *retColTerrain);

	//�����֣���ֻ����һ�£�������ص�4��p������x ����y �ֱ��룺
	//0,0
	//0,1
	//1,0
	//1,1
	inline static void foreachRect(int maxX, int maxY, std::function<void(int x, int y)> p);

protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:
	void clearGround();

	Maps m_maps;
	struct TERRAIN
	{
		std::string name;
		Spirit spirit;
		int tankPass, HP;
	};
	std::vector<TERRAIN> m_terrains;

	TankFactory m_tankFactory;
	std::map<Tank *, CAMP> m_tanks;

	std::set<Missile *> m_missiles;
};


SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect);
SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint);
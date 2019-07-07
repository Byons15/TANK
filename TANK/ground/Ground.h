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
		//坦克被销毁，这个事件一定是跟着Tank::ATTACKTANK事件的。
		//code：被摧毁的坦克的阵营。
		//data1：被摧毁的坦克型号。
		//data2: 被摧毁的坦克的计分板..
		DESTORYTANK = 0x999,
	};

	Ground(Renderer *renderer);
	~Ground();

	Maps &maps() {return m_maps;}
	const ROAD &colMap() const {return m_colMap;}

	const TankFactory &tankFactory() {return m_tankFactory;}
	
	//成功返回指针，返回0表示复活点上有其他坦克，被占用了，生成失败。 
	Tank* addTank(Tank::MODEL tankModel, Tank::CAMP camp, int bindIndex);

	void addMissile(Missile *m);

	//攻击坦克。
	//坦克已摧毁返回0， 坦克已受伤返回1， 坦克处在无敌状态返回-1
	int attackTank(Driver * aggressor, Tank *tank, int power);

	int attackTerrain(const SDL_Point &pos, int power);

	//摧毁坦克， 这个是强制的
	void destoryTank(Tank * target);

	void destoryBase();

	void destoryTerrain(const SDL_Point &pos);

	const std::set<Tank *> &tankList() const { return m_tanks; }

	//坦克碰撞检查
	//没有碰撞返回0， 与地形发生碰撞返回-1，与其他坦克发生碰撞则返回-2,并将被撞坦克赋值retColDest
	int positionTest(Tank *tank, const SDL_Point &pixelPos, Tank ** retColDest);

	int findTankOnRect(Tank *tank, const SDL_Rect &rect, std::vector<Tank *>& outResult);

	//炮弹碰撞检查
	//没有碰撞返回0， 与地形发生碰撞返回-1，与坦克发生碰撞则返回-2, 边界碰撞返回-3
	int MissilepositionUpdate(Missile *m);

	//投机取巧之作，遍历maxX和maxY组成的矩阵，将每个位置回调p
	static void foreachRect(int maxX, int maxY, std::function<void(int x, int y)> p);

	//更新碰撞映射
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
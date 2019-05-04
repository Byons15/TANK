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

	Maps &maps() {
		return m_maps;
	}
	const std::array<std::array<bool, MAP_SIZE>, MAP_SIZE> &colMap() const {
		return m_colMap;
	}

	const TankFactory &tankFactory() {
		return m_tankFactory;
	}
	
	//成功返回指针，返回0表示复活点上有其他坦克，被占用了，生成失败。 
	Tank* addTank(int tankModel, CAMP camp, int bindIndex);

	void addMissile(Missile *m);

	//攻击坦克。
	//坦克已摧毁返回0， 坦克已受伤返回1， 坦克处在无敌状态返回-1
	int attackTank(Tank *tank, int power);

	int attackTerrain(const SDL_Point &pos, int power);

	//摧毁坦克， 这个是强制的
	void destoryTank(Tank *tank);

	void destoryBase();

	void destoryTerrain(const SDL_Point &pos);

	const std::map<Tank *, CAMP> &tankList() const {
		return m_tanks;
	}

	//坦克碰撞检查
	//没有碰撞返回0， 与地形发生碰撞返回-1，与其他坦克发生碰撞则返回-2,并将被撞坦克赋值retColDest
	int positionTest(Tank *tank, const SDL_Point &pixelPos, Tank ** retColDest);

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

private:
	void clearGround();

	Maps m_maps;

	TankFactory m_tankFactory;
	std::map<Tank *, CAMP> m_tanks;
	std::array<std::array<bool, MAP_SIZE>, MAP_SIZE> m_colMap;

	std::list<Missile *> m_missiles;
};


SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect);
SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint);
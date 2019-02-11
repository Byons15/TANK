#pragma once
#include "../Scene.h"
#include "Maps.h"
#include "../Event.h"
#include <map>
#include "TankFactory.h"

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

	Ground(Renderer *renderer);
	~Ground();

	virtual void open(void *data, int code) override;
	virtual void close() override;

	//地形图
	Maps &maps() {
		return m_maps;
	}
	Tank* radar(int x, int y);
	const TankFactory &tankFactory() {
		return m_tankFactory;
	}

	Tank* addTank(int tankModel, CAMP camp, int bindIndex);

	//攻击坦克。
	//坦克已摧毁返回0， 坦克已受伤返回1， 坦克处在无敌状态返回-1
	int attackTank(Tank *tank, int power);

	//摧毁坦克， 这个是强制的
	void destoryTank(Tank *tank);

protected: 
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private: friend class Tank;
	//更新坦克位置。
	//此功能只能由Tank类调用，其他人调用会打乱战场布局
	//成功返回0， 发生碰撞无法更新则返回-1
	int tankPositionUpdate(Tank *tank, const SDL_Point &pos);

private:
	void clearGround();

	Maps m_maps;
	std::array<std::array<Tank *, MAP_SIZE>, MAP_SIZE> m_radar;  //雷达保存了哪个网格上存在坦克。
	std::map<Tank *, SDL_Point> m_tanks;  //val值保存的是坦克在战场网格中的位置的左上角。

	TankFactory m_tankFactory;
};


SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect);
SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint);
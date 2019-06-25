#pragma once
#include <array>
#include <SDL_rect.h>
#include "../TANKS.h"
#include <list>
#include "../ground/Mover.h"

typedef std::array<std::array<bool, MAP_SIZE>, MAP_SIZE> ROAD;

class Navigator
{
public:
	Navigator(const ROAD * road);

	//开始导航
	//p1 : 开始位置
	//p2 : 目的地
	//返回：0开始导航， !0没有找到路线。
	int navigation(const SDL_Point &p1, const SDL_Point &p2);

	int navigationToCollimationLine(const SDL_Point &p1, const SDL_Point &CollimationLine);

	//重置导航器。
	void resetNavigator();

	//获取移动到下一个位置的需要移动的方向。
	//current : 当前位置。
	//outDirection : 存放移动到下一个位置需要移动的方向。
	//返回 ：0成功， -1导航已经结束了，outDirection没有意义。 -2情况不妙。
	int nextPosition(const SDL_Point &current, Mover::DIRECTION& outDirection);

	size_t lenght() {
		return m_path.size();
	}

	bool state() {
		return !m_path.empty();
	}
	~Navigator();

private:
	int findLine(const SDL_Point &p1, const SDL_Point &line);
	int findWay(const SDL_Point &p1, const SDL_Point &p2);
	void createDirection(const SDL_Point & p1, const SDL_Point & p2);
	bool collisionCheck(const SDL_Point &p);
	bool borderCheck(const SDL_Point &p);
	void createpreferredDirection(const SDL_Point &p1, const SDL_Point &p2);

	std::list<SDL_Point> m_path;
	std::list<SDL_Point>::iterator m_currentPosition;
	std::array<SDL_Point, 4> m_preferredDirection;
	Mover::DIRECTION m_currentDirection;
	static ROAD sm_visiteSign;
	const ROAD *m_road;
};


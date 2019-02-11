#include "Ground.h"
#include "../Renderer.h"
#include "Tank.h"

Ground::Ground(Renderer * renderer)
	:Scene(renderer, {0, 0, GRID_SIZE * MAP_SIZE, GRID_SIZE * MAP_SIZE }), m_tankFactory(renderer)
{
	//TODO::
}

Ground::~Ground()
{
	//TODO::
}


Tank * Ground::radar(int x, int y)
{
	return m_radar[x][y];
}

Tank* Ground::addTank(int tankModel, CAMP camp, int bindIndex)
{

	//取得复活点的位置。
	SDL_Point p;
	if (camp == ALLISE) {
		p = m_maps.alliesBind(bindIndex);
	}
	else if (camp == ENEMY) {
		p = m_maps.enemyBind(bindIndex);
	}
	else {
		throw std::out_of_range("超出阵营选择范围");
	}
	
	//注意转换成像素位置
	Tank *t = new Tank(this, tankModel, pixelToGroundPoint(p));

	//如果复活点被占用，则生成坦克失败。
	if (tankPositionUpdate(t, p) == -1) {
		delete t;
		return 0;
	}

	return t;
}

void Ground::destoryTank(Tank * tank)
{
	SDL_Point p = m_tanks[tank];
	//将坦克从雷达中抹去。
	fourSquareTraversal([this, &p](int x, int y) {m_radar[p.x + x][p.y + y] = 0; });
	
	m_tanks.erase(tank);

	delete tank;
}

int Ground::tankPositionUpdate(Tank * tank, const SDL_Point & pixelPos)
{
	//TODO::碰撞检测。

	return 0;
}

//不知道这玩意inline了编译器会不会听。
inline void Ground::fourSquareTraversal(std::function<void(int x, int y)> p)
{
	for (int x = 0; x != 2; ++x) {
		for (int y = 0; y != 2; ++y) {
			p(x, y);
		}
	}
}

void Ground::clearGround()
{
	for (auto &iter : m_radar) {
		iter.fill(0);
	}

	for (auto &iter : m_tanks) {
		delete iter.first;
	}
	m_tanks.clear();
}

SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint)
{
	SDL_Point  ret;
	ret.y = pixelPoint.y / GRID_SIZE;
	ret.x = pixelPoint.x / GRID_SIZE;
	return ret;
}

SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect)
{
	SDL_Point upperLeft = pixelToGroundPoint({ pixelRect.x, pixelRect.y });
	SDL_Point lowRight = pixelToGroundPoint({ pixelRect.x + pixelRect.w, pixelRect.y + pixelRect.h });

	SDL_Rect rect;
	rect.x = upperLeft.x;
	rect.y = upperLeft.y;
	rect.w = ++lowRight.x - upperLeft.x;
	rect.h = ++lowRight.y - upperLeft.y;

	return rect;
}
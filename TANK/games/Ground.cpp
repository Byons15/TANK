#include "Ground.h"
#include "../Renderer.h"
#include "Tank.h"
#include "../FileLoader.h"
#include <sstream>

Ground::Ground(Renderer * renderer)
	:Scene(renderer, {0, 0, GRID_SIZE * MAP_SIZE, GRID_SIZE * MAP_SIZE }), m_tankFactory(renderer)
{
	//载入地形数据
	std::vector<std::string> data;
	fileLoad("terrain", &data);
	for (auto &s : data) {
		std::istringstream is(s);
		TERRAIN t;
		is >> t.name >> t.tankPass >> t.HP;
		m_terrains.push_back(t);
	}
}

Ground::~Ground()
{
	clearGround();
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
	if (t->setPosition(p) == -1) {
		delete t;
		return 0;
	}

	return t;
}

int Ground::attackTank(Tank * tank, int power)
{
	auto result = tank->beHit(tank, power);
	if (!result)
		destoryTank(tank);

	return result;
}

void Ground::destoryTank(Tank * tank)
{
	m_tanks.erase(tank);

	delete tank;
}

int Ground::tankColCheck(Tank * tank, const SDL_Point & pixelPos, Tank ** retColDest)
{
	SDL_Rect rect = pixelToGroundRect({pixelPos.x, pixelPos.y, Tank::colSize, Tank::colSize});

	//边界检查.
	if (rect.x < 0 || rect.y < 0 || rect.x + rect.w > MAP_SIZE || rect.y + rect.h > MAP_SIZE) {
		return -1;
	}

	//检查地形碰撞。 
	int result = 0;
	foreachRect(rect.w, rect.y, 
		[this, &rect, &result] (int x, int y) -> void 
		{
			if (m_maps(rect.x + x, rect.y + y)) {
				if (m_terrains[m_maps(rect.x + x, rect.y + y) - 1].tankPass == 0) {
					result = -1;
					return;
				}
			}
		});
	if (result == -1) {
		return -1;
	}

	//检查坦克碰撞。
	SDL_Rect r1 = {pixelPos.x, pixelPos.y, Tank::colSize, Tank::colSize};
	SDL_Rect r2 = {0, 0, Tank::colSize, Tank::colSize };
	for (auto &t : m_tanks) {
		r2.x = t->position().x;
		r2.y = t->position().y;
		if (SDL_HasIntersection(&r1, &r2) != SDL_FALSE && t != tank) {
			*retColDest = t;
			return -2;
		}
	}
		
	//能走到这里说明没有发生碰撞
	return 0;
}

//不知道这玩意inline了编译器会不会听。
inline void Ground::foreachRect(int maxX, int maxY, std::function<void(int x, int y)> p)
{
	for (int x = 0; x != maxX; ++x) {
		for (int y = 0; y != maxY; ++y) {
			p(x, y);
		}
	}
}

void Ground::clearGround()
{
	for (auto &iter : m_tanks) {
		delete iter;
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
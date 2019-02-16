#include "Ground.h"
#include "../Renderer.h"
#include "Tank.h"
#include "../FileLoader.h"
#include <sstream>
#include "../Director.h"
#include <list>

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
		t.spirit.setScene(this);
		t.spirit.setAnimation(t.name);
		m_terrains.push_back(t);
	}
}

Ground::~Ground()
{
	clearGround();
}

void Ground::open(void * data, int code)
{
	setUserEventHook(Missile::END_BOOM);
	setState(true);
}

void Ground::close()
{
	unsetUserEventHook(Missile::END_BOOM);
	setState(false);
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

	m_tanks[t] = camp;

	return t;
}

void Ground::addMissile(Missile * m)
{
	m_missiles.insert(m);
}

int Ground::attackTank(Tank * tank, int power)
{
	auto result = tank->beHit(tank, power);
	if (!result)
		destoryTank(tank);

	return result;
}

int Ground::attackTerrain(const SDL_Point & pos, int power)
{
	auto terrain = m_maps(pos.x, pos.y);
	if (terrain != 0) {
		if (m_terrains[terrain - 1].HP <= power)
			destoryTerrain(pos);
		else {
			return -2;
		}
	}
	else {
		return -1;
	}
	return 0;
}

void Ground::destoryTank(Tank * tank)
{
	SDL_UserEvent user;
	user.type = DESTORYTANK;
	user.code = m_tanks[tank];
	user.data1 = reinterpret_cast<void *>(tank->model());

	m_tanks.erase(tank);
	delete tank;
}

void Ground::destoryBase()
{
	//TODO::
	close();
}

void Ground::destoryTerrain(const SDL_Point & pos)
{
	if (m_maps(pos.x, pos.y) - 1 == 1) {
		destoryBase();
	}

	m_maps.setTerrain(pos.x, pos.y);
}

int Ground::tankColCheck(Tank * tank, const SDL_Point & pixelPos, Tank ** retColDest)
{
	*retColDest = 0;

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
		r2.x = t.first->position().x;
		r2.y = t.first->position().y;
		if (SDL_HasIntersection(&r1, &r2) != SDL_FALSE && t.first != tank) {
			*retColDest = t.first;
			return -2;
		}
	}
		
	//能走到这里说明没有发生碰撞
	return 0;
}

int Ground::missileColCheck(Missile * m, const SDL_Point & checkPos, Tank ** retColDest, SDL_Point *retColTerrain)
{
	*retColDest = 0;
	*retColTerrain = {-1, -1};
	auto rect = pixelToGroundRect({checkPos.x, checkPos.y, Missile::missileSize, Missile::missileSize });

	//边界检查.
	if (rect.x < 0 || rect.y < 0 || rect.x + rect.w > MAP_SIZE || rect.y + rect.h > MAP_SIZE) {
		return -3;
	}

	//检查地形碰撞。 
	int result = 0;
	foreachRect(rect.w, rect.y,
		[this, &rect, &result, &retColTerrain](int x, int y) -> void
	{
		if (m_maps(rect.x + x, rect.y + y)) {
			if (m_terrains[m_maps(rect.x + x, rect.y + y) - 1].tankPass == 0) {
				result = -1;
				retColTerrain->x = rect.x + x;
				retColTerrain->y = rect.y + y;
				return;
			}
		}
	});
	if (result == -1) {
		return -1;
	}

	//检查坦克碰撞。
	SDL_Rect r1 = { checkPos.x, checkPos.y, Missile::missileSize, Missile::missileSize };
	SDL_Rect r2 = { 0, 0, Tank::colSize, Tank::colSize };
	for (auto &t : m_tanks) {
		r2.x = t.first->position().x;
		r2.y = t.first->position().y;
		if (SDL_HasIntersection(&r1, &r2) != SDL_FALSE) {
			*retColDest = t.first;
			return -2;
		}
	}

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

void Ground::update(Uint32 time)
{
	for (auto &t : m_terrains) {
		t.spirit.updateFrames(time);
	}

	for (auto &t : m_tanks) {
		t.first->update(time);
		t.first->updateFrames(time);
	}

	for (auto &m : m_missiles) {
		m->update(time);
		m->updateFrames(time);
	}
}

int Ground::render()
{
	//渲染位于坦克下方的地形并挑拣出位于坦克上方的地形
	std::list<SDL_Point> lastRender;
	for (auto x = 0; x != MAP_SIZE; ++x) {
		for (auto y = 0; y != MAP_SIZE; ++y) {
			if (m_terrains[m_maps(x, y) - 1].tankPass == 1) {
				lastRender.push_back({ x, y });
			}
			else {
				m_terrains[m_maps(x, y) - 1].spirit.renderFrame({ x * GRID_SIZE, y *GRID_SIZE });
			}
		}
	}

	for (auto &t : m_tanks) {
		t.first->render();
	}

	for (auto &m : m_missiles) {
		m->render();
	}

	//渲染位于坦克上方的地形。
	for (auto &p : lastRender) {
		m_terrains[m_maps(p.x, p.y) - 1].spirit.renderFrame({ p.x * GRID_SIZE, p.y *GRID_SIZE });
	}

	return 0;
}

void Ground::userEventHookProc(const SDL_UserEvent & user)
{
	switch (user.type)
	{
	case Missile::END_BOOM: 
	{
		Missile * m = reinterpret_cast<Missile *>(user.data1);
		m_missiles.erase(m);
		delete m;
	}
		break;
	default:
		break;
	}
}

void Ground::clearGround()
{
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
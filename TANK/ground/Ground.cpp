#include "Ground.h"
#include "../Renderer.h"
#include "Tank.h"
#include "../FileLoader.h"
#include <sstream>
#include "../Director.h"
#include <list>

Ground::Ground(Renderer * renderer)
	:Scene(renderer, { 0, 0, GRID_SIZE * MAP_SIZE, GRID_SIZE * MAP_SIZE }), m_tankFactory(renderer), m_maps(this)
{
	setBackdropColor(SDL_Color{0, 0, 0, SDL_ALPHA_OPAQUE});
	//设置坦克工厂
	Tank::setFactory(&m_tankFactory);
}

Ground::~Ground()
{
	clearGround();
}

void Ground::open(void * data, int code)
{
	installUserEventHook();
}

void Ground::close()
{
	uninstallUserEventHook();
}

Tank* Ground::addTank(Tank::MODEL tankModel, Tank::CAMP camp, int bindIndex)
{

	//取得复活点的位置。
	SDL_Point p;
	if (camp == ALLISE) {
		p = m_maps.alliesBindPosition(bindIndex);
	}
	else if (camp == ENEMY) {
		p = m_maps.enemyBindPosition(bindIndex);
	}
	else {
		throw std::out_of_range("超出阵营选择范围");
	}
	
	//注意转换成像素位置
	Tank *t = new Tank(this, camp, tankModel, p);

	//如果复活点被占用，则生成坦克失败。
	if (t->setGroundPosition(p) == -1) {
		delete t;
		return 0;
	}

	m_tanks.insert(t);

	return t;
}

void Ground::addMissile(Missile * m)
{
	m_missiles.push_back(m);
	m->setDestoryIterator(--m_missiles.end());
}

int Ground::attackTank(Tank * aggressor, Tank * tank, int power)
{
	auto result = tank->beHit(aggressor, power);

	if (!result) 
		destoryTank(tank);

	return result;
}

int Ground::attackTerrain(const SDL_Point & pos, int power)
{
	auto terrain = m_maps(pos.x, pos.y);
	if (terrain != 0) {
		if (m_maps.terrainData(terrain - 1).HP <= power && m_maps.terrainData(terrain - 1).HP != 0)
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

void Ground::destoryTank(Tank * target)
{
	m_tanks.erase(target);
	delete target;
}

void Ground::destoryBase()
{
	//TODO::
	m_maps.destoryBase();
	//TODO::
	//close();
}

void Ground::destoryTerrain(const SDL_Point & pos)
{
	if (m_maps(pos.x, pos.y) == 1) {
		destoryBase();
	}

	m_maps.setTerrain(pos.x, pos.y);
}

int Ground::positionTest(Tank * tank, const SDL_Point & pixelPos, Tank ** retColDest)
{
	*retColDest = 0;

	SDL_Rect rect = pixelToGroundRect({pixelPos.x, pixelPos.y, Tank::colSize + 1, Tank::colSize + 1});

	//边界检查.
	if (rect.x < 0 || rect.y < 0 || rect.x + rect.w > MAP_SIZE || rect.y + rect.h > MAP_SIZE) {
		return -1;
	}

	//检查地形碰撞。 
	int result = 0;
	foreachRect(rect.w, rect.h, 
		[this, &rect, &result] (int x, int y) -> void 
		{
			if (m_maps(rect.x + x, rect.y + y)) {
				if (m_maps.terrainData(m_maps(rect.x + x, rect.y + y) - 1).tankPass == 0) {
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
		r2.x = t->pixelPosition().x;
		r2.y = t->pixelPosition().y;
		if (SDL_HasIntersection(&r1, &r2) != SDL_FALSE && t != tank) {
			*retColDest = t;
			return -2;
		}
	}
		
	//能走到这里说明没有发生碰撞
	return 0;
}

int Ground::findTankOnRect(Tank * tank, const SDL_Rect & rect, std::vector<Tank*>& outResult)
{
	outResult.clear();

	for (auto &t : m_tanks) {
		if (SDL_PointInRect(&t->position(), &rect))
			if (t != *m_tanks.find(tank))
				outResult.push_back(t);
	}

	return outResult.size();
}

int Ground::MissilepositionUpdate(Missile * m)
{
	
	auto rect = pixelToGroundRect({ m->position().x, m->position().y, Missile::missileSize, Missile::missileSize });

	//边界检查.
	if (rect.x < 0 || rect.y < 0 || rect.x + rect.w > MAP_SIZE || rect.y + rect.h > MAP_SIZE) {
		return -3;
	}
	int result = 0;

	//检查地形碰撞。 
	foreachRect(rect.w, rect.h,
		[this, &rect, &result, &m](int x, int y) -> void
	{
		if (m_maps(rect.x + x, rect.y + y)) {
			if (m_maps.terrainData(m_maps(rect.x + x, rect.y + y) - 1).tankPass == 0) {
				attackTerrain({ rect.x + x, rect.y + y }, m->power());
				result = -1;
			}
		}
	});

	//坦克和子弹的碰撞体积。
	SDL_Rect r1 = { m->position().x, m->position().y, Missile::missileSize, Missile::missileSize };
	SDL_Rect r2 = { 0, 0, Tank::colSize, Tank::colSize };

	for (auto iter = m_tanks.begin(); iter != m_tanks.end();) {
		if (*iter != m->sender()) {
			r2.x = (*iter)->pixelPosition().x;
			r2.y = (*iter)->pixelPosition().y;
			if (SDL_HasIntersection(&r1, &r2) != SDL_FALSE) {
				auto d = iter;
				++iter;
				attackTank(m->sender(), *d, m->power());
				result = -2;
				continue;
			}
		}
		++iter;
	}

	return result;
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

void Ground::updateColMap(int x, int y)
{
	m_colMap[x][y] = (m_maps(x, y)) ? !m_maps.terrainData(m_maps(x, y) - 1).tankPass : false;
}

void Ground::update(Uint32 time)
{
	for (auto &t : m_maps.m_terrainPool) {
		t.spirit.updateFrames(time);
	}

	for (auto &t : m_tanks) {
		t->update(time);
		t->updateFrames(time);
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
	bool baseRender = false;
	for (auto x = 0; x != MAP_SIZE; ++x) {
		for (auto y = 0; y != MAP_SIZE; ++y) {
			if (m_maps(x, y)) {
				if (m_maps.terrainData(m_maps(x, y) - 1).tankPass == 1) {
					lastRender.push_back({ x, y });
				}
				else {
					if (baseRender && m_maps(x, y) == 1)
						continue;
					
					m_maps.terrainData(m_maps(x, y) - 1).spirit.renderFrame({ x * GRID_SIZE, y *GRID_SIZE });
					if (m_maps(x, y) == 1)
						baseRender = true;
				}
			}
		}
	}

	for (auto &t : m_tanks) {
		t->render();
	}

	for (auto &m : m_missiles) {
		m->render();
	}

	//渲染位于坦克上方的地形。
	for (auto &p : lastRender) {
		m_maps.terrainData(m_maps(p.x, p.y) - 1).spirit.renderFrame({ p.x * GRID_SIZE, p.y *GRID_SIZE });
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
		m_missiles.erase(m->destoryIterator());
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
		delete iter;
	}
	m_tanks.clear();
}

SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint)
{
	return { pixelPoint.x / GRID_SIZE, pixelPoint.y / GRID_SIZE };
}

SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect)
{
	SDL_Rect rect;
	rect.x = pixelRect.x / GRID_SIZE;
	rect.y = pixelRect.y / GRID_SIZE;
	rect.w = (pixelRect.x + pixelRect.w) % GRID_SIZE ? 
			 (pixelRect.x + pixelRect.w) / GRID_SIZE + 1 - rect.x : (pixelRect.x + pixelRect.w) / GRID_SIZE - rect.x;
	rect.h = (pixelRect.y + pixelRect.h) % GRID_SIZE ?
			 (pixelRect.y + pixelRect.h) / GRID_SIZE + 1 - rect.y : (pixelRect.y + pixelRect.h) / GRID_SIZE - rect.y;

	return rect;
}
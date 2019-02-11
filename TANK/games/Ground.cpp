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

	//ȡ�ø�����λ�á�
	SDL_Point p;
	if (camp == ALLISE) {
		p = m_maps.alliesBind(bindIndex);
	}
	else if (camp == ENEMY) {
		p = m_maps.enemyBind(bindIndex);
	}
	else {
		throw std::out_of_range("������Ӫѡ��Χ");
	}
	
	//ע��ת��������λ��
	Tank *t = new Tank(this, tankModel, pixelToGroundPoint(p));

	//�������㱻ռ�ã�������̹��ʧ�ܡ�
	if (tankPositionUpdate(t, p) == -1) {
		delete t;
		return 0;
	}

	return t;
}

void Ground::destoryTank(Tank * tank)
{
	SDL_Point p = m_tanks[tank];
	//��̹�˴��״���Ĩȥ��
	fourSquareTraversal([this, &p](int x, int y) {m_radar[p.x + x][p.y + y] = 0; });
	
	m_tanks.erase(tank);

	delete tank;
}

int Ground::tankPositionUpdate(Tank * tank, const SDL_Point & pixelPos)
{
	//TODO::��ײ��⡣

	return 0;
}

//��֪��������inline�˱������᲻������
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
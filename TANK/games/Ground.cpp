#include "..\Ground.h"
#include "../Renderer.h"

Ground::Ground(Renderer * renderer)
	:Scene(renderer, {0, 0, GRID_SIZE * MAP_SIZE, GRID_SIZE * MAP_SIZE }), m_tankFactory(renderer)
{
	
}

Ground::~Ground()
{
}


void Ground::addTank(int tankModel, CAMP camp, int bindIndex)
{
	SDL_Point p;
	if (camp == ALLISE) {
		p = m_maps.alliesBind(bindIndex);
	}
	else if (camp == ENEMY) {
		p = m_maps.enemyBind(bindIndex);
	}
	else {
		throw std::out_of_range("³¬³öÕóÓªÑ¡Ôñ·¶Î§");
	}
}

void Ground::clearRadar()
{
	for (auto &iter : m_radar) {
		iter.fill(0);
	}
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
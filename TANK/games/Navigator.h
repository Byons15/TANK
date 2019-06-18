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

	//��ʼ����
	//p1 : ��ʼλ��
	//p2 : Ŀ�ĵ�
	//���أ�0��ʼ������ !0û���ҵ�·�ߡ�
	int startNavigation(const SDL_Point &p1, const SDL_Point &p2);

	//���õ�������
	void resetNavigator();

	//��ȡ�ƶ�����һ��λ�õ���Ҫ�ƶ��ķ���
	//current : ��ǰλ�á�
	//outDirection : ����ƶ�����һ��λ����Ҫ�ƶ��ķ���
	//���� ��0�ɹ��� -1�����Ѿ������ˣ�outDirectionû�����塣 -2������
	int nextPosition(const SDL_Point &current, Mover::DIRECTION& outDirection);

	bool state() {
		return !m_path.empty();
	}
	~Navigator();

private:
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


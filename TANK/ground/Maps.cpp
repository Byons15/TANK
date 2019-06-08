#include "Maps.h"
#include "../FileLoader.h"
#include <sstream>
#include <random>
#include <SDL_timer.h>
#include "Ground.h"
#include "../Timer.h"

Maps::Maps(Ground *ground)
	:m_ground(ground)
{
	//�����������
	std::vector<std::string> data;
	fileLoad("terrain", &data);
	for (auto &s : data) {
		std::istringstream is(s);
		TERRAIN t;
		is >> t.name >> t.tankPass >> t.HP;
		t.spirit.setScene(ground);
		t.spirit.setAnimation(t.name);
		m_terrainPool.push_back(t);
	}

	clearMap();
}

int Maps::createMaps(int level)
{
	if (loadMaps(level) == -1)
		return createMaps();
	return 0;
}

inline void Maps::setMap(int x, int y, int index)
{
	m_map[x][y] = index;
	m_ground->updateColMap(x, y);
}

int Maps::loadMaps(int level)
{

	clearMap();

	//�������ݡ�
	std::string groupName = "level " + std::to_string(level);
	std::vector<std::string> data;
	if (fileLoad(groupName, &data) != MAP_SIZE)
		return -1;
	
	m_reservePoint[0] = { -1, -1 };
	//��ȡ���ݲ������ͼ��
	size_t enemyRefreshIndex = 0;
	for (int ln = 0; ln != MAP_SIZE; ++ln) {
		std::istringstream is(data[ln]);
		for (int col = 0; col != MAP_SIZE; ++col) {
			int index;
			is >> index;
			//��������ݱ�ʶ��-1��ʾP1ˢ�µ㣬 -2��ʾP2ˢ�µ㣬 -3��ʾ�о�ˢ�µ㡣
			if (index < 0) {
				
				//��������еı�ʶ��0������������о�ˢ�µ㡣˵һ�¾��ǵо�ˢ�µ�������ұ�����3����
				//��0��ʾ������ǿյġ�
				//֮�������if����Ǳ���P1 P2ˢ�µ�ġ�
				if (index == -3) {
					m_reservePoint[enemyRefreshIndex + alliesBindCount + 1] = { col, ln };
					enemyRefreshIndex++;
				}

				if (index == -1)
					m_reservePoint[1] = { col, ln };

				if (index == -2)
					m_reservePoint[2] = { col, ln };

				setMap(col, ln, 0);
			}
			else {
				setMap(col, ln, index);
			}

			//�������λ�á�
			if (m_reservePoint[0].x == -1 && m_reservePoint[0].y == -1 && m_map[col][ln] == 1) {
				m_reservePoint[0] = { col, ln };
			}
		}
	}
	
	

	return 0;
}

int Maps::createMaps()
{
	clearMap();
	
	//�򵥵�����ֲ����Ρ�
	std::default_random_engine e(Timer::SDLTimer());
	std::uniform_int_distribution<> dis(0, m_terrainPool.size());
	for (size_t x = 0; x != m_map.size(); ++x) {
		for (size_t y = 0; y != m_map[x].size(); ++y) {
			m_map[x][y] = dis(e);
			m_map[x][y] = (m_map[x][y] == TAG_BASE) ? 0 : m_map[x][y];
		}
	}

	//ƽ�������ͼ��
	smoothMap();
	smoothMap();

	//TODO�������������ͼ�Ա�֤����ˢ�µ��ܵ�����ص�λ�á�

	//���û���Χǽ
	//�Ȱѻ��ص�λ����Ҳ���ש��û��ϵ������֮�����ػ��滻����
	for (auto x = 11; x != 15; ++x) {
		for (auto y = 23; y != 26; ++y) {
			m_map[x][y] = 2;
		}
	}

	//����ˢ�µ��Լ�����λ�á�
	initReservePoint();

	//������ײģ�͡�
	for (auto x = 0; x != m_map.size(); ++x) {
		for (auto y = 0; y != m_map[x].size(); ++y) {
			setMap(x, y, m_map[x][y]);
		}
	}

	return 0;
}

void Maps::destoryBase()
{
	for (auto x = m_reservePoint[0].x; x != m_reservePoint[0].x + 2; ++x) {
		for (auto y = m_reservePoint[0].y; y != m_reservePoint[0].y + 2; ++y) {
			setMap(x, y, 0);
		}
	}
}

int Maps::operator()(int x, int y)
{
	if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
		throw std::out_of_range("���ʵ�ͼԽ��");

	return m_map[x][y];
}

int Maps::setTerrain(int x, int y, uint32_t terrainIndex)
{
	if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
		throw std::out_of_range("���ʵ�ͼԽ��");
	
	for (auto &p : m_reservePoint) {
		if (x >= p.x  && x < p.x + 2 &&
			y >= p.y && y < p.y + 2)
			return -1;
	}

	setMap(x, y, terrainIndex);

	return 0;
}

void Maps::initReservePoint()
{
	for (auto x = 0; x != 2; ++x) {
		for (auto y = 0; y != 2; ++y) {

			for (auto i = 1; i != m_reservePoint.size(); ++i) {
				m_map[m_reservePoint[i].x + x][m_reservePoint[i].y + y] = 0;
			}

			setMap(x + m_reservePoint[0].x, y + m_reservePoint[0].y, TAG_BASE);
		}
	}
}

void Maps::clearMap()
{
	for (int x = 0; x != m_map.size(); ++x) {
		for (int y = 0; y != m_map[x].size(); ++y) {
			setMap(x, y, 0);
		}
	}

	//Ĭ�ϵо������
	m_reservePoint[3]  = { 0, 0 };
	m_reservePoint[4]  = { 12, 0 };
	m_reservePoint[5]  = { 24, 0 };

	//Ĭ���Ѿ������
	m_reservePoint[1] = { 9, 24 };
	m_reservePoint[2] = { 15, 24 };

	//Ĭ�ϻ���λ�á�
	m_reservePoint[0] = { 12, 24 };
	setMap(12, 24, TAG_BASE);
	setMap(13, 24, TAG_BASE);
	setMap(12, 25, TAG_BASE);
	setMap(13, 25, TAG_BASE);
}

Maps::~Maps()
{
}

void Maps::smoothMap()
{	
	//ƽ��ÿ��λ�á�
	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> dest = m_map;
	for (auto x = 0; x != MAP_SIZE; ++x) {
		for (auto y = 0; y != MAP_SIZE; ++y) {
			smoothPoint(dest, x, y);
		}
	}

	m_map = dest;
}

void Maps::smoothPoint(std::array<std::array<int, MAP_SIZE>, MAP_SIZE> & destMap, int x, int y)
{
	std::pair<int, int> much{ 0, 0 };

	//�ҳ��Ź������������ĵ��Ρ�
	for (size_t t = 0; t <= m_terrainPool.size(); ++t) {
		int weight = 0;

		//������x��yΪ���ĵľŹ��� 
		SDL_Rect rect;
		rect.x = (x - 1 < 0) ? x : x - 1;
		rect.y = (y - 1 < 0) ? y : y - 1;
		rect.w = (x + 1 >= MAP_SIZE) ? x : x + 1;
		rect.h = (y + 1 >= MAP_SIZE) ? y : y + 1;
		for (auto i = rect.x; i <= rect.w; ++i) {
			for (auto j = rect.y; j <= rect.h; ++j) {
				if (t == m_map[i][j])
					++weight;
			}
		}

		if (much.second < weight) {
			much = {t, weight };
		}
		else if(much.second == weight){
			std::default_random_engine e(Timer::SDLTimer());
			std::uniform_int_distribution<> dis(1, 10);
			if (dis(e) > 5) {
				much = { t, weight };
			}
		}
	}

	much.first = (much.first == 1) ? 0 : much.first;
	destMap[x][y] = much.first;
}

int Maps::navigationToBase(const SDL_Point & beginPos)
{
	
	return 0;
}

int Maps::navigationToPosition(const SDL_Point & src, const SDL_Point & dest)
{
	return 0;
}

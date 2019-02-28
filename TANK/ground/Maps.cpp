#include "Maps.h"
#include "../FileLoader.h"
#include <sstream>
#include <random>
#include <SDL_timer.h>

Maps::Maps()
{
	clearMap();
}

int Maps::loadMaps(int level)
{

	clearMap();

	//�������ݡ�
	std::string groupName = "level " + std::to_string(level);
	std::vector<std::string> data;
	if (fileLoad(groupName, &data) != MAP_SIZE)
		return -1;
	
	m_basePoint = { -1, -1 };
	//��ȡ���ݲ������ͼ��
	size_t enemyRefreshIndex = 0;
	for (int ln = 0; ln != MAP_SIZE; ++ln) {
		std::istringstream is(data[ln]);
		for (int col = 0; col != MAP_SIZE; ++col) {
			is >> m_map[col][ln];

			//��������ݱ�ʶ��-1��ʾP1ˢ�µ㣬 -2��ʾP2ˢ�µ㣬 -3��ʾ�о�ˢ�µ㡣
			if (m_map[col][ln] < 0) {
				m_map[col][ln] = 0;

				//��������еı�ʶ��0������������о�ˢ�µ㡣˵һ�¾��ǵо�ˢ�µ�������ұ�����3����
				//��0��ʾ������ǿյġ�
				//֮�������if����Ǳ���P1 P2ˢ�µ�ġ�
				if (m_map[col][ln] == -3) 
					m_enemyBind[enemyRefreshIndex++] = { col, ln };

				if (m_map[col][ln] == -1) 
					m_alliesBind[0] = { col, ln };

				if (m_map[col][ln] == -2) 
					m_alliesBind[1] = { col, ln };
			}

			//�������λ�á�
			if (m_basePoint.x == -1 && m_basePoint.y == -1 && m_map[col][ln] == 1) {
				m_basePoint = { col, ln };
			}
		}
	}
	
	

	return 0;
}

int Maps::createMaps(Uint32 terrainMaxIndex)
{
	clearMap();
	
	//�򵥵�����ֲ����Ρ�
	std::default_random_engine e(SDL_GetTicks());
	std::uniform_int_distribution<> dis(1, terrainMaxIndex);
	for (size_t x = 0; x != m_map.size(); ++x) {
		for (size_t y = 0; y != m_map[x].size(); ++y) {
			m_map[x][y] = dis(e);
			m_map[x][y] = (m_map[x][y] == TAG_BASE) ? 0 : m_map[x][y];
		}
	}

	//ƽ�������ͼ��
	natureMap(terrainMaxIndex);
	natureMap(terrainMaxIndex);

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

	return 0;
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
	
	m_map[x][y] = terrainIndex;
	return 0;
}

void Maps::initReservePoint()
{
	for (auto x = 0; x != 2; ++x) {
		for (auto y = 0; y != 2; ++y) {

			for (auto &p : m_enemyBind) {
				m_map[p.x + x][p.y + y] = 0;
			}
			for (auto &p : m_alliesBind) {
				m_map[p.x + x][p.y + y] = 0;
			}
			m_map[x + m_basePoint.x][y + m_basePoint.y] = TAG_BASE;
		}
	}
}

void Maps::clearMap()
{
	for (auto &iter : m_map) {
		iter.fill(0);
	}

	//Ĭ��̹��ˢ�µ㡣
	m_enemyBind[0]  = { 0, 0 };
	m_enemyBind[1]  = { 12, 0 };
	m_enemyBind[2]  = { 24, 0 };
	m_alliesBind[0] = { 9, 24 }; 
	m_alliesBind[1] = { 15, 24 };

	//Ĭ�ϻ���λ�á�
	m_map[12][24] = m_map[13][24] = m_map[12][25] = m_map[13][25] = TAG_BASE;
	m_basePoint = { 12, 24 };
}

Maps::~Maps()
{
}

void Maps::natureMap(Uint32 terrainMaxIndex)
{	
	//ƽ��ÿ��λ�á�
	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> dest = m_map;
	for (auto x = 0; x != MAP_SIZE; ++x) {
		for (auto y = 0; y != MAP_SIZE; ++y) {
			smoothPoint(dest, x, y, terrainMaxIndex);
		}
	}

	m_map = dest;
}

void Maps::smoothPoint(std::array<std::array<int, MAP_SIZE>, MAP_SIZE> & destMap, int x, int y, int terrainMaxIndex)
{
	std::pair<int, int> much{ 0, 0 };

	//�ҳ��Ź������������ĵ��Ρ�
	for (auto t = 0; t <= terrainMaxIndex; ++t) {
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
			std::default_random_engine e(SDL_GetTicks());
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

#include "Maps.h"
#include "../FileLoader.h"
#include <sstream>

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
}

Maps::~Maps()
{
}

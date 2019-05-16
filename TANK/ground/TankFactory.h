#pragma once
#include "../FileLoader.h"
#include <map>
#include "../Renderer.h"

class Tank;

/*
̹�˹������ļ��ж�ȡ�����湹��̹������Ҫ�����ݣ� ��Щ������������Tankʱ�� Tank������������ṩ��ģ�ͱ�Ŵ�����ȡ����Ҫ�����ݡ�
̹��ģ�ͱ�Ŵ�0��ʼ�� ���ܴ��ڻ����modelCount�������صļ�������Ŷ�Ӧ�ľ����ͺţ��뵽�ļ�"TANKS.dat"�µ�<Tank>������ġ�
*/
class TankFactory
{
	friend class Tank;

public:
	TankFactory(Renderer *renderer);
	TankFactory() = delete;
	~TankFactory();

	//����ģ�ͼ�����
	int modelCount() const {
		return m_modelIndex.size();
	}

private:
	struct TANKDAT
	{
		std::vector<Animation> form;
		Animation rewardForm, invincibleForm;
		int maxHP, killScore;
		float speeds;
	};

	const TANKDAT &findTankData(const std::string &name);
	const TANKDAT &findTankData(int model) {
		return findTankData(m_modelIndex[model]);
	}

	std::map<std::string, TANKDAT> m_pool;
	std::vector<std::string> m_modelIndex;
	Renderer *m_renderer;
};


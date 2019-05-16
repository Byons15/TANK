#pragma once
#include "../FileLoader.h"
#include <map>
#include "../Renderer.h"

class Tank;

/*
坦克工厂从文件中读取并保存构造坦克所需要的数据， 这些数据在您构造Tank时， Tank类会自行依据提供的模型编号从这里取得需要的数据。
坦克模型编号从0开始， 不能大于或等于modelCount（）返回的计数。编号对应的具体型号，请到文件"TANKS.dat"下的<Tank>分组查阅。
*/
class TankFactory
{
	friend class Tank;

public:
	TankFactory(Renderer *renderer);
	TankFactory() = delete;
	~TankFactory();

	//返回模型计数。
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


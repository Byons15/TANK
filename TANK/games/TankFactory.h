#pragma once
#include "../FileLoader.h"
#include <map>
#include "../Renderer.h"

class Tank;

class TankFactory
{
	friend class Tank;

public:
	TankFactory() = delete;
	~TankFactory();

	int modelCount() const {
		return m_modelIndex.size();
	}

private:
	struct TANKDAT
	{
		std::vector<Animation> form;
		Animation rewardForm, invincibleForm;
		int maxHP, speeds;
	};

	TankFactory(Renderer *renderer);
	const TANKDAT &findTankData(const std::string &name);
	const TANKDAT &findTankData(int model) {
		return findTankData(m_modelIndex[model]);
	}

	std::map<std::string, TANKDAT> m_pool;
	std::vector<std::string> m_modelIndex;
	Renderer *m_renderer;
};


#include "TankFactory.h"
#include <sstream>

TankFactory::TankFactory(Renderer *renderer)
	:m_renderer(renderer)
{
	std::vector<std::string> data;

	fileLoad("tank", &data);

	for (auto &line : data) {
		std::istringstream is(line);
		std::string name;
		is >> name;
		auto &dat = m_pool[name];
		m_modelIndex.push_back(name);

		is >> dat.speeds >> dat.killScore >> dat.maxHP;

		std::string str;
		for (int i = 0; i != dat.maxHP; ++i) {
			is >> str;
			dat.form.push_back(renderer->animationFactory().findAnimation(str));
		}
		dat.form.resize(dat.maxHP, dat.form[0]);
		
		is >> str;
		dat.rewardForm = (str == "DEFAULT") ? dat.form[0] : renderer->animationFactory().findAnimation(str);

		is >> str;
		dat.invincibleForm = (str == "DEFAULT") ? dat.form[0] : renderer->animationFactory().findAnimation(str);
	}
}

const TankFactory::TANKDAT & TankFactory::findTankData(const std::string & name)
{
	auto iter = m_pool.find(name);
	if (iter == m_pool.end()) {
		std::out_of_range("有个蛇皮这个型号的坦克");
	}
	return iter->second;
}


TankFactory::~TankFactory()
{
}

#include "TankFactory.h"

TankFactory::TankFactory()
{
	std::vector<std::string> data;

	if (!fileLoad("objects.data", "tank", &data)) {
		std::runtime_error("�޷���Object");
	}
}


TankFactory::~TankFactory()
{
}

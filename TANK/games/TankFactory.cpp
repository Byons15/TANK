#include "TankFactory.h"

TankFactory::TankFactory()
{
	std::vector<std::string> data;

	if (!fileLoad("objects.data", "tank", &data)) {
		std::runtime_error("无法打开Object");
	}
}


TankFactory::~TankFactory()
{
}

#include "../FileLoader.h"
#include <fstream>

unsigned int fileLoad(const std::string & groupName, std::vector<std::string>* retData)
{
	std::ifstream file("TANK.dat");
	if (!file) {
		return 0;
	}
	std::string group = '<' + groupName + '>';

	std::string line;
	while (std::getline(file, line)) {
		if (line == group) {
			std::getline(file, line);
			while (line != "<end>") {
				retData->push_back(line);
				std::getline(file, line);
			}
			break;
		}
	}

	file.close();

	return retData->size();
}

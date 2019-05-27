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
	int count = 0;
	while (std::getline(file, line)) {
		if (line == group) {
			std::getline(file, line);
			while (line != "<end>") {
				retData->push_back(line);
				++count;
				std::getline(file, line);
			}
			break;
		}
	}

	file.close();

	return count;
}

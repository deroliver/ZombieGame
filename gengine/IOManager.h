#pragma once

#include <vector>

namespace gengine {

class IOManager {

public:
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
};

}



#pragma once
#include <sstream>
#include <string>

#include "../model/Scene.h"

namespace ReadFile {

	struct FileData {
		Scene scene;
		std::string output_filename;
		int max_recursion_depth;
	};

	// From HW2
	bool readvals(std::stringstream& s, const int numvals, float* values);
	void ignoreCommandWithMessage(std::string cmd, int number_of_arguments, float* arguments);
	FileData readfile(const char* filename);

}

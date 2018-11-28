#include "test.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<double>
vec_from_file(std::string path)
{
	std::vector<double> data;
	std::ifstream infile(path);

	if (infile.fail()) {
		std::cerr << "File '" << path << "' doesn't exist, exiting"
		          << std::endl;
		exit(1);
	}

	double val;
	while (infile >> val)
		data.push_back(val);

	return data;
}

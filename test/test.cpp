#include "test.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<double>
vec_from_file(std::string path)
{
	std::vector<double> data;
	std::ifstream infile(path);

	double val;
	while (infile >> val)
		data.push_back(val);

	return data;
}

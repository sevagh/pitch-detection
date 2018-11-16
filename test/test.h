#ifndef TEST_H
#define TEST_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<double>
vec_from_file(std::string path)
{
	std::vector<double> data;
	std::string line;

	std::ifstream infile(path);

	while (std::getline(infile, line))
		data.push_back(std::stod(line));

	return data;
}

#endif /* TEST_H */

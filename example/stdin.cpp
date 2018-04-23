#include "example.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <pitch_detection.h>
#include <utility>
#include <vector>

int
main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "Usage: stdin <algo> <sample_rate>" << std::endl;
		return -1;
	}

	std::vector<double> x;
	double n;
	while (std::cin >> n)
		x.push_back(n);

	std::string algo(argv[1]);
	int sample_rate = std::stoi(argv[2]);

	double pitch =
	    pitch_algorithms[pitch_types[algo]](x, sample_rate);

	std::cout << "Size: " << x.size() << "\tpitch: " << pitch << std::endl;
	return 0;
}

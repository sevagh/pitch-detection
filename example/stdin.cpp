#include "stdin.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <gflags/gflags.h>
#include <iostream>
#include <limits>
#include <pitch_detection.h>
#include <utility>
#include <vector>

DEFINE_double(sample_rate, 48000, "Input sample rate");
DEFINE_string(algo, "mpm", "Algorithm to test");

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	std::vector<double> x;
	double n;
	while (std::cin >> n)
		x.push_back(n);

	std::cout << x.size() << std::endl;

	double pitch =
	    pitch_algorithms[pitch_types[FLAGS_algo]](x, FLAGS_sample_rate);

	std::cout << "Pitch: " << pitch << std::endl;
	return 0;
}

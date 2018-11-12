#include "example.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <gflags/gflags.h>
#include <iostream>
#include <limits>
#include <pitch_detection.h>
#include <utility>
#include <vector>

DEFINE_uint64(sample_rate, 48000, "Sample rate in Hz");
DEFINE_validator(sample_rate, [](const char *flagname, uint64_t value) {
	if (value >= 0)
		return true;
	return false;
});

DEFINE_string(algo, "mpm", "Algorithm to test");
DEFINE_validator(algo, [](const char *flagname, const std::string &value) {
	if (pitch_types.find(value) != pitch_types.end()) {
		return true;
	}
	return false;
});

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	std::vector<double> data;
	double n;
	while (std::cin >> n)
		data.push_back(n);

	auto SIZE = int(data.size());

	double pitch =
	    pitch_algorithms[pitch_types[FLAGS_algo]](data, FLAGS_sample_rate);

	std::cout << "Size: " << SIZE << "\tpitch: " << pitch << std::endl;
	return 0;
}

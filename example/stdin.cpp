#include "example.h"
#include <fstream>
#include <gflags/gflags.h>
#include <iostream>
#include <limits>
#include <pitch_detection.h>
#include <util.h>
#include <utility>
#include <vector>

DEFINE_uint64(sample_rate, 48000, "Sample rate in Hz");
DEFINE_validator(sample_rate,
    [](const char *flagname, uint64_t value) { return (value >= 0); });

DEFINE_string(algo, "mpm", "Algorithm to test");
DEFINE_validator(algo, [](const char *flagname, const std::string &value) {
	return (pitch_funcs.find(value) != pitch_funcs.end());
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

	double pitch = pitch_funcs[FLAGS_algo](data, FLAGS_sample_rate);

	std::cout << "Size: " << SIZE << "\tpitch: " << pitch << std::endl;

	auto closest = util::closest_note(pitch);

	std::cerr << "closest note: " << std::get<0>(closest) << " ("
	          << std::get<1>(closest) << ")" << std::endl;

	return 0;
}

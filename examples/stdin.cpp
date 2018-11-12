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

DEFINE_bool(plot_lags, false, "Output sinewave data with lags to stdout");

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

	if (FLAGS_plot_lags) {
		std::vector<double> orig(2 * SIZE);
		std::vector<double> quarter_lag(2 * SIZE);
		std::vector<double> half_lag(2 * SIZE);
		std::vector<double> full_lag(2 * SIZE);

		for (int i = 0; i < 2 * SIZE; ++i) {
			if (i < SIZE)
				orig[i] = data[i];
			if ((i > SIZE / 4) && (i < SIZE / 4 + SIZE)) {
				quarter_lag[i] = data[i - SIZE / 4];
			}
			if ((i > SIZE / 2) && (i < SIZE / 2 + SIZE)) {
				half_lag[i] = data[i - SIZE / 2];
			}
			if (i > SIZE) {
				full_lag[i] = data[i - SIZE];
			}
		}

		for (int i = 0; i < 2 * SIZE; ++i) {
			std::cout << orig[i] << "\t" << quarter_lag[i] << "\t"
			          << half_lag[i] << "\t" << full_lag[i] << std::endl;
		}
	}

	double pitch =
	    pitch_algorithms[pitch_types[FLAGS_algo]](data, FLAGS_sample_rate);

	std::cout << "Size: " << SIZE << "\tpitch: " << pitch << std::endl;
	return 0;
}

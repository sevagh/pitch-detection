#include <cstdlib>
#include <cstring>
#include <fstream>
#include <gflags/gflags.h>
#include <iostream>
#include <limits>
#include <pitch_detection.hpp>
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
	double pitch = 0.0f;
	if (FLAGS_algo == "mpm") {
		pitch = get_pitch_mpm(x, FLAGS_sample_rate);
	} else if (FLAGS_algo == "autocorrelation") {
		pitch = get_pitch_autocorrelation(x, FLAGS_sample_rate);
	} else if (FLAGS_algo == "yin") {
		pitch = get_pitch_yin(x, FLAGS_sample_rate);
	} else {
		std::cout << FLAGS_algo << " is not a valid algo\n";
		std::exit(-1);
	}
	std::cout << "pitch: " << pitch << std::endl;
	return 0;
}

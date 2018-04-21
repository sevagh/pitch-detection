#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cerrno>
#include <functional>
#include <iostream>
#include <pitch_detection.h>
#include <utility>
#include <vector>
#include <gflags/gflags.h>
#include "example.h"

DEFINE_double(freq, -1, "Sinewave frequency hz");
DEFINE_uint64(size, 4096, "Sinewave size");
DEFINE_uint64(sample_rate, 48000, "Sample rate hz");
DEFINE_string(algo, "mpm", "Algorithm to test");

static std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate);

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	bool freq_valid = (FLAGS_freq > 0.0);
	if (!freq_valid) {
		std::cerr << "Please define valid --freq" << std::endl;
		return -1;
	}

	auto x = generate_sinewave(2*FLAGS_size, FLAGS_freq, FLAGS_sample_rate);

	double pitch = pitch_algorithms[pitch_types[FLAGS_algo]](x, FLAGS_sample_rate);

	std::cout << "Freq: " << FLAGS_freq << "\tpitch: " << pitch << std::endl;
	return 0;
}

static std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate)
{
	size_t lut_size = size / 4;

	std::vector<int> lut{};
	std::vector<double> tone_single_channel{};

	double doublef = (double)frequency;
	double delta_phi = doublef * lut_size * 1.0 / sample_rate;
	double phase = 0.0;

	for (int i = 0; i < signed(lut_size); ++i) {
		lut.push_back((int)roundf(0x7FFF * sinf(2.0 * M_PI * i / lut_size)));
	}

	for (int i = 0; i < signed(size / 2); ++i) {
		int val = double(lut[(int)phase]);
		tone_single_channel.push_back(val);
		phase += delta_phi;
		if (phase >= lut_size)
			phase -= lut_size;
	}

	return tone_single_channel;
}

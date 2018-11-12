#include "example.h"
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <float.h>
#include <fstream>
#include <functional>
#include <gflags/gflags.h>
#include <iostream>
#include <pitch_detection.h>
#include <utility>
#include <vector>

DEFINE_double(frequency, -1.0, "Sinewave frequency in Hz");
DEFINE_validator(frequency, [](const char *flagname, double value) {
	if (value >= 0.0)
		return true;
	return false;
});

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

DEFINE_int32(size, 4096, "Sine wave size (single channel)");
DEFINE_validator(size, [](const char *flagname, int value) {
	if (value >= 0)
		return true;

	return false;
});

// forward declaration
static std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate);

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	auto data =
	    generate_sinewave(2 * FLAGS_size, FLAGS_frequency, FLAGS_sample_rate);

	if ((FLAGS_algo == "mpm") &&
	    !(FLAGS_size && !(FLAGS_size & (FLAGS_size - 1))))
		std::cerr << "FFTS performs better with power-of-two sizes"
		          << std::endl;

	double pitch =
	    pitch_algorithms[pitch_types[FLAGS_algo]](data, FLAGS_sample_rate);

	std::cerr << "Size: " << FLAGS_size << "\tfreq: " << FLAGS_frequency
	          << "\tpitch: " << pitch << std::endl;
	return 0;
}

static std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate)
{
	size_t lut_size = size / 4;

	std::vector<int> lut{};
	double *_tone_single_channel = (double *)malloc(sizeof(double) * size / 2);

	double doublef = (double)frequency;
	double delta_phi = doublef * lut_size * 1.0 / sample_rate;
	double phase = 0.0;

	for (int i = 0; i < signed(lut_size); ++i) {
		lut.push_back((int)roundf(0x7FFF * sinf(2.0 * M_PI * i / lut_size)));
	}

	double min = DBL_MAX;
	double max = -DBL_MAX;
	for (int i = 0; i < signed(size / 2); ++i) {
		int val = double(lut[(int)phase]);
		if (val > max) {
			max = val;
		}
		if (val < min) {
			min = val;
		}
		_tone_single_channel[i] = val;
		phase += delta_phi;
		if (phase >= lut_size)
			phase -= lut_size;
	}

	std::vector<double> tone_single_channel(
	    _tone_single_channel, _tone_single_channel + size / 2);

	return tone_single_channel;
}

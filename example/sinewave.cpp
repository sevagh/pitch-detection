#include "./time_template.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <gflags/gflags.h>
#include <iostream>
#include <limits>
#include <pitch_detection.hpp>
#include <utility>
#include <vector>

DEFINE_double(freq, -1, "Sinewave frequency");
DEFINE_uint64(size, 4096, "Sinewave size");
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

	auto x = generate_sinewave(FLAGS_size, FLAGS_freq, 48000);
	double pitch = 0.0f;
	std::function<double(std::vector<double> &, int)> pitch_func;

	if (FLAGS_algo == "mpm") {
		pitch_func = get_pitch_mpm;
	} else if (FLAGS_algo == "autocorrelation") {
		pitch_func = get_pitch_autocorrelation;
	} else if (FLAGS_algo == "yin") {
		pitch_func = get_pitch_yin;
	} else {
		std::cout << FLAGS_algo << " is not a valid algo\n";
		std::exit(-1);
	}

	//std::cout << measure<>::execution(1000, pitch_func, x, 48000) << std::endl;

	pitch = pitch_func(x, 48000);
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

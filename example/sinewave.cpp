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
#include <random>
#include <string>
#include <utility>
#include <vector>

DEFINE_double(frequency, -1.0, "Sinewave frequency in Hz");
DEFINE_validator(frequency, [](const char *flagname, double value) {
	if (value >= 0.0)
		return true;
	return false;
});

DEFINE_double(noise, 0.0, "Noise to introduce in %");
DEFINE_validator(noise, [](const char *flagname, double value) {
	if ((value >= 0.0) && (value <= 100.0))
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

DEFINE_bool(plot, false, "Output sinewave data to stdout");
DEFINE_bool(plot_lags, false, "Output sinewave data with lags to stdout");
DEFINE_bool(quiet, false, "Suppress most outputs");

static std::vector<double>
generate_sinewave(
    size_t size, double frequency, int sample_rate, double noise_perc);

int
main(int argc, char **argv)
{
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	auto data = generate_sinewave(
	    2 * FLAGS_size, FLAGS_frequency, FLAGS_sample_rate, FLAGS_noise);

	if (FLAGS_plot) {
		if (FLAGS_plot_lags) {
			std::cerr << "--plot and --plot_lags are mutually exclusive"
			          << ::std::endl;

			return -1;
		}

		for (auto x : data)
			std::cout << x << std::endl;
	}

	if (FLAGS_plot_lags) {
		std::vector<double> orig(2 * FLAGS_size);
		std::vector<double> quarter_lag(2 * FLAGS_size);
		std::vector<double> half_lag(2 * FLAGS_size);
		std::vector<double> full_lag(2 * FLAGS_size);

		for (int i = 0; i < 2 * FLAGS_size; ++i) {
			if (i < FLAGS_size)
				orig[i] = data[i];
			if ((i > FLAGS_size / 4) && (i < FLAGS_size / 4 + FLAGS_size)) {
				quarter_lag[i] = data[i - FLAGS_size / 4];
			}
			if ((i > FLAGS_size / 2) && (i < FLAGS_size / 2 + FLAGS_size)) {
				half_lag[i] = data[i - FLAGS_size / 2];
			}
			if (i > FLAGS_size) {
				full_lag[i] = data[i - FLAGS_size];
			}
		}

		for (int i = 0; i < 2 * FLAGS_size; ++i) {
			std::cout << orig[i] << "\t" << quarter_lag[i] << "\t"
			          << half_lag[i] << "\t" << full_lag[i] << std::endl;
		}
	}

	if (!FLAGS_quiet && (FLAGS_algo == "mpm") &&
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
generate_sinewave(
    size_t size, double frequency, int sample_rate, double noise_perc)
{
	int num_elements_to_skip = int(noise_perc / 100.0 * size / 2);

	std::random_device rnd_device;
	std::mt19937 mersenne_engine{rnd_device()}; // Generates random integers
	std::uniform_int_distribution<int> dist(0, int(size / 2) - 1);

	auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

	std::vector<int> elements_to_skip(num_elements_to_skip);
	std::generate(begin(elements_to_skip), end(elements_to_skip), gen);

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

	std::uniform_real_distribution<double> amplitude_dist(min, max);
	auto amplitude_gen = [&amplitude_dist, &mersenne_engine]() {
		return amplitude_dist(mersenne_engine);
	};

	std::vector<double> tone_single_channel(
	    _tone_single_channel, _tone_single_channel + size / 2);

	for (auto skip : elements_to_skip) {
		tone_single_channel.at(skip) = amplitude_gen();
	}

	return tone_single_channel;
}

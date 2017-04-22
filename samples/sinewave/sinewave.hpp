#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <gflags/gflags.h>
#include <vector>
#include <mpm.hpp>
#include <yin.hpp>
#include <autocorrelation.hpp>

pitch_fn get_pitch_fn(std::string algo)
{
	if (algo == "mpm") {
		return get_pitch_mpm;
	} else if (algo == "autocorrelation") {
		return get_pitch_autocorrelation;
	} else if (algo == "yin") {
		return get_pitch_yin;
	} else {
		std::cout << algo << " is not a valid algo\n";
		std::exit(-1);
	}
}

DEFINE_double(freq, -1, "Sinewave frequency");
DEFINE_uint64(size, 4096, "Sinewave size");
DEFINE_string(algo, "mpm", "Algorithm to test");
DEFINE_string(path, DEFAULT_PATH, "Audio file path");

static bool validate_path(const char* flagname, const std::string& path)
{
	std::ifstream file(path);
	return file.good();
}

int main(int argc, char **argv)
{
    if (!gflags::RegisterFlagValidator(&FLAGS_path, &validate_path)) {
        std::cerr << "Failed to register path validator" << std::endl;
        exit(1);
    }
	gflags::SetUsageMessage("help\n");
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	bool freq_valid = (FLAGS_freq > 0.0);
	bool path_valid = (FLAGS_path.compare(DEFAULT_PATH) != 0);
	if (freq_valid == path_valid) {
		std::cerr << "Please define valid --freq OR --path" << std::endl;
		return -1;
	} else if (freq_valid) {
		auto x = generate_sinewave(FLAGS_size, FLAGS_freq, 48000);
		double pitch = get_pitch_fn(FLAGS_algo)(x, 48000);
		std::cout << "Freq: " << FLAGS_freq << "\tpitch: " << pitch << std::endl;
	} else if (path_valid) {
#ifdef FFMPEG_ENABLED
		read_audio_file(FLAGS_path, get_pitch_fn(FLAGS_algo));
#else
		std::cerr << "Compiled without ffmpeg/libav, cannot use this feature\n";
		std::exit(-1);
#endif
	}
	return 0;
}

std::vector<double> generate_sinewave(size_t size, double frequency, int sample_rate)
{
	size_t lut_size = size/4;

	std::vector<int> lut {};
	std::vector<double> tone_single_channel {};

	double doublef = (double) frequency;
	double delta_phi = doublef * lut_size * 1.0 / sample_rate;
	double phase = 0.0;

	for (int i = 0; i < signed(lut_size); ++i) {
		lut.push_back((int) roundf(0x7FFF * sinf(2.0 * M_PI * i / lut_size)));
	}

	for (int i = 0; i < signed(size/2); ++i) {
		int val = double(lut[(int) phase]);
		tone_single_channel.push_back(val);
		phase += delta_phi;
		if (phase >= lut_size) phase -= lut_size;
	}

	return tone_single_channel;
}

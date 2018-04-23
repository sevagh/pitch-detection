#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <cerrno>
#include <functional>
#include <iostream>
#include <pitch_detection.h>
#include <utility>
#include <vector>
#include "example.h"
extern "C" {
#include <fftw3.h>
}

static std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate);

int
main(int argc, char **argv)
{
	if (argc != 5) {
		std::cerr << "Usage: sinewave <freq> <algo> <size> <sample_rate>" << std::endl;
		return -1;
	}

	double freq = std::stod(argv[1]);
	std::string algo(argv[2]);
	int size = std::stoi(argv[3]);
	int sample_rate = std::stoi(argv[4]);

	auto x = generate_sinewave(2*size, freq, sample_rate);

	double pitch = pitch_algorithms[pitch_types[algo]](x, sample_rate);

	std::cout << "Size: " << size << "\tfreq: " << freq << "\tpitch: " << pitch << std::endl;
	return 0;
}

static std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate)
{
	size_t lut_size = size / 4;

	std::vector<int> lut{};
	double *_tone_single_channel = fftw_alloc_real(size / 2);

	double doublef = (double)frequency;
	double delta_phi = doublef * lut_size * 1.0 / sample_rate;
	double phase = 0.0;

	for (int i = 0; i < signed(lut_size); ++i) {
		lut.push_back((int)roundf(0x7FFF * sinf(2.0 * M_PI * i / lut_size)));
	}

	for (int i = 0; i < signed(size / 2); ++i) {
		int val = double(lut[(int)phase]);
		_tone_single_channel[i] = val;
		phase += delta_phi;
		if (phase >= lut_size)
			phase -= lut_size;
	}

	std::vector<double> tone_single_channel(_tone_single_channel, _tone_single_channel + size / 2);
	return tone_single_channel;
}

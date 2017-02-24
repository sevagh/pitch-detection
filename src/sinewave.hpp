#ifndef SINEWAVE_H
#define SINEWAVE_H

#include <vector>
#include <cmath>
#include <cstring>

inline std::vector<double> generate_sinewave(size_t size, double frequency, int sample_rate)
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

#endif /* SINEWAVE_H */

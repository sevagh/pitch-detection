#ifndef MPM_H
#define MPM_H

#include <float.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <float.h>
#include <iostream>
#include <numeric>
#include <complex>
#include "parabolic_interpolation.hpp"

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

extern "C" {
#include <xcorr.h>
}

static std::vector<double> normalized_square_difference(std::vector<double>
							audio_buffer)
{
	int size = audio_buffer.size();
	int size2 = 2*size-1;

	std::vector<std::complex<double>> acf(size2);
	std::vector<double> acf_real{};

	xcorr_fftw_r2c(&audio_buffer[0], &audio_buffer[0], &acf[0], size);

	for (auto it = acf.begin() + size2/2; it != acf.end(); ++it)
		acf_real.push_back((*it).real()/acf[size2/2].real());

	return acf_real;
}

static std::vector<int> peak_picking(std::vector<double> nsdf)
{
	std::vector<int> max_positions{};
	int pos = 0;
	int curMaxPos = 0;
	ssize_t size = nsdf.size();

	while (pos < (size - 1) / 3 && nsdf[pos] > 0) pos++;
	while (pos < size - 1 && nsdf[pos] <= 0.0) pos++;

	if (pos == 0) pos = 1;

	while (pos < size - 1) {
		if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1]) {
			if (curMaxPos == 0) {
				curMaxPos = pos;
			} else if (nsdf[pos] > nsdf[curMaxPos]) {
				curMaxPos = pos;
			}
		}
		pos++;
		if (pos < size - 1 && nsdf[pos] <= 0) {
			if (curMaxPos > 0) {
				max_positions.push_back(curMaxPos);
				curMaxPos = 0;
			}
			while (pos < size - 1 && nsdf[pos] <= 0.0) {
				pos++;
			}
		}
	}
	if (curMaxPos > 0) {
		max_positions.push_back(curMaxPos);
	}
	return max_positions;
}

inline double get_pitch_mpm(std::vector<double> audio_buffer, int sample_rate)
{
	std::vector<double> nsdf = normalized_square_difference(audio_buffer);
	std::vector<int> max_positions = peak_picking(nsdf);
	std::vector<std::pair<double, double>> estimates;

	double highestAmplitude = -DBL_MAX;

	for (int i : max_positions) {
		highestAmplitude = std::max(highestAmplitude, nsdf[i]);
		if (nsdf[i] > SMALL_CUTOFF) {
			auto x = parabolic_interpolation(nsdf, i);
			estimates.push_back(x);
			highestAmplitude = std::max(highestAmplitude, std::get<1>(x));
		}
	}

	if (estimates.empty()) return -1;

	double actualCutoff = CUTOFF * highestAmplitude;
	double period = 0;

	for (auto i : estimates) {
		if (std::get<1>(i) >= actualCutoff) {
			period = std::get<0>(i);
			break;
		}
	}

	double pitchEstimate = (sample_rate / period);
	return (pitchEstimate > LOWER_PITCH_CUTOFF) ? pitchEstimate : -1;
}

#endif /* MPM_H */

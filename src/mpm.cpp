#include <float.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <float.h>
#include <iostream>
#include <numeric>
#include <complex>
#include "./parabolic_interpolation.hpp"
#include "./constants.hpp"
#include <pitch_detection.hpp>

extern "C" {
#include <xcorr.h>
}

static std::vector<double> normalized_square_difference(const std::vector<double>&
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

static std::vector<int> peak_picking(const std::vector<double>& nsdf)
{
	std::vector<int> max_positions{};
	int pos = 0;
	int cur_max_pos = 0;
	ssize_t size = nsdf.size();

	while (pos < (size - 1) / 3 && nsdf[pos] > 0) pos++;
	while (pos < size - 1 && nsdf[pos] <= 0.0) pos++;

	if (pos == 0) pos = 1;

	while (pos < size - 1) {
		if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1]) {
			if (cur_max_pos == 0) {
				cur_max_pos = pos;
			} else if (nsdf[pos] > nsdf[cur_max_pos]) {
				cur_max_pos = pos;
			}
		}
		pos++;
		if (pos < size - 1 && nsdf[pos] <= 0) {
			if (cur_max_pos > 0) {
				max_positions.push_back(cur_max_pos);
				cur_max_pos = 0;
			}
			while (pos < size - 1 && nsdf[pos] <= 0.0) {
				pos++;
			}
		}
	}
	if (cur_max_pos > 0) {
		max_positions.push_back(cur_max_pos);
	}
	return max_positions;
}

double get_pitch_mpm(const std::vector<double>& audio_buffer, int sample_rate)
{
	std::vector<double> nsdf = normalized_square_difference(audio_buffer);
	std::vector<int> max_positions = peak_picking(nsdf);
	std::vector<std::pair<double, double>> estimates;

	double highest_amplitude = -DBL_MAX;

	for (int i : max_positions) {
		highest_amplitude = std::max(highest_amplitude, nsdf[i]);
		if (nsdf[i] > MPM_SMALL_CUTOFF) {
			auto x = parabolic_interpolation(nsdf, i);
			estimates.push_back(x);
			highest_amplitude = std::max(highest_amplitude, std::get<1>(x));
		}
	}

	if (estimates.empty()) return -1;

	double actual_cutoff = MPM_CUTOFF * highest_amplitude;
	double period = 0;

	for (auto i : estimates) {
		if (std::get<1>(i) >= actual_cutoff) {
			period = std::get<0>(i);
			break;
		}
	}

	double pitch_estimate = (sample_rate / period);
	return (pitch_estimate > MPM_LOWER_PITCH_CUTOFF) ? pitch_estimate : -1;
}

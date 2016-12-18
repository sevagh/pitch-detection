#include <float.h>
#include <algorithm>
#include "autocorrelation.h"
#include "mpm.h"
#include <vector>
#include <tuple>
#include <numeric>

mpm::mpm(int size, int sample_rate)
{
	mpm::sample_rate = sample_rate;
	mpm::size = size;

}

static std::tuple<double, double> parabolic_interpolation(double nsdfa, double nsdfb, double nsdfc, int tau)
{
	double bValue = tau;
	double bottom = nsdfc + nsdfa - 2 * nsdfb;
	if (bottom == 0.0) {
		return {bValue, nsdfb};
	} else {
		double delta = nsdfa - nsdfc;
		return {bValue + delta / (2 * bottom), nsdfb - delta * delta / (8 * bottom)};
	}
}

std::vector<double> mpm::nsdf_time_domain(double *audio_buffer)
{
	std::vector<double> nsdf{};
	int tau;
	for (tau = 0; tau < size; tau++) {
		double acf = 0;
		double divisorM = 0;
		for (int i = 0; i < size - tau; i++) {
			acf += audio_buffer[i] * audio_buffer[i + tau];
			divisorM += audio_buffer[i] * audio_buffer[i] + audio_buffer[i + tau] * audio_buffer[i + tau];
		}
		nsdf.push_back(2 * acf / divisorM);
	}
	return nsdf;
}

static std::vector<int> peak_picking(std::vector<double> nsdf)
{
	std::vector<int> max_positions{};
	int pos = 0;
	int curMaxPos = 0;
	ssize_t size = nsdf.size();

	while (pos < (size - 1) / 3 && nsdf[pos] > 0) {
		pos++;
	}

	while (pos < size - 1 && nsdf[pos] <= 0.0) {
		pos++;
	}

	if (pos == 0) {
		pos = 1;
	}

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

double mpm::get_pitch(double *audio_buffer)
{
	std::vector<double> nsdf = mpm::nsdf_time_domain(audio_buffer);
	std::vector<int> max_positions = peak_picking(nsdf);
	std::vector<std::tuple<double, double>> estimates;

	double highestAmplitude = -DBL_MAX;

	for (int i : max_positions) {
		highestAmplitude = MAX(highestAmplitude, nsdf[i]);

		if (nsdf[i] > SMALL_CUTOFF) {
			auto x = parabolic_interpolation(nsdf[i-1], nsdf[i], nsdf[i+1], i);
			estimates.push_back(x);
			highestAmplitude = MAX(highestAmplitude, std::get<1>(x));
		}
	}

	if (estimates.empty()) {
		return -1;
	} else {
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
}

mpm::~mpm()
{
}

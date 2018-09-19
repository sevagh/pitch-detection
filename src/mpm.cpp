#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstring>
#ifndef PORTABLE_XCORR
#include <ffts/ffts.h>
#endif /* PORTABLE_XCORR */
#include <float.h>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <vector>

static std::vector<double>
acorr_r(const std::vector<double> &signal)
{
#ifndef PORTABLE_XCORR
	int N = signal.size();
	int N2 = 2 * N;

	auto fft_forward = ffts_init_1d(N2, FFTS_FORWARD);
	auto fft_backward = ffts_init_1d(N2, FFTS_BACKWARD);

	std::vector<std::complex<float>> signalb_ext(N2);

	for (int i = 0; i < N; i++)
		signalb_ext[i] = {float(signal[i]), 0.0};

	std::vector<std::complex<float>> outb(N2);
	std::vector<std::complex<float>> out(N2);
	std::vector<std::complex<float>> result(N2);

	ffts_execute(fft_forward, signalb_ext.data(), outb.data());

	std::complex<float> scale = {1.0f / (float)N2, 0.0};
	for (int i = 0; i < N; ++i)
		out[i] = outb[i] * std::conj(outb[i]) * scale;

	ffts_execute(fft_backward, out.data(), result.data());

	ffts_free(fft_forward);
	ffts_free(fft_backward);

	std::vector<double> normalized_result(N, 0.0);
	for (int i = 0; i < N; ++i)
		normalized_result[i] = std::real(result[i]) / std::real(result[0]);
	return normalized_result;
#else
	std::vector<double> nsdf{};
	int size = signed(signal.size());
	int tau;
	for (tau = 0; tau < size; tau++) {
		double acf = 0;
		double divisorM = 0;
		for (int i = 0; i < size - tau; i++) {
			acf += signal[i] * signal[i + tau];
			divisorM +=
			    signal[i] * signal[i] + signal[i + tau] * signal[i + tau];
		}
		nsdf.push_back(2 * acf / divisorM);
	}
	return nsdf;
#endif /* PORTABLE_XCORR */
}

static std::vector<int>
peak_picking(const std::vector<double> &nsdf)
{
	std::vector<int> max_positions{};
	int pos = 0;
	int cur_max_pos = 0;
	ssize_t size = nsdf.size();

	while (pos < (size - 1) / 3 && nsdf[pos] > 0)
		pos++;
	while (pos < size - 1 && nsdf[pos] <= 0.0)
		pos++;

	if (pos == 0)
		pos = 1;

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

double
get_pitch_mpm(const std::vector<double> &data, int sample_rate)
{
	std::vector<double> nsdf = acorr_r(data);
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

	if (estimates.empty())
		return -1;

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

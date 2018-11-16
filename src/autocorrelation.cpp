#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <vector>
#ifndef PORTABLE_XCORR
#include <ffts/ffts.h>
#endif /* PORTABLE_XCORR */

std::vector<double>
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

static double
get_acf_periodicity(const std::vector<double> &data)
{
	int size = data.size();
	int total_peak_bin_index = 0;
	int occurences = 0;

	for (int i = 1; i < size - 1; i++) {
		if ((data[i] > data[i - 1]) && (data[i]) > data[i + 1]) {
			occurences += 1;
			total_peak_bin_index = i;
		}
	}

	return (double)(total_peak_bin_index) / (double)occurences;
}

double
pitch::autocorrelation(const std::vector<double> &data, int sample_rate)
{
	double peak_bin_index_periodicity = get_acf_periodicity(acorr_r(data));

	return (sample_rate / peak_bin_index_periodicity);
}

#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

namespace
{
namespace pyin_consts
{

static const float Beta_Distribution[100] = {0.012614, 0.022715, 0.030646,
    0.036712, 0.041184, 0.044301, 0.046277, 0.047298, 0.047528, 0.047110,
    0.046171, 0.044817, 0.043144, 0.041231, 0.039147, 0.036950, 0.034690,
    0.032406, 0.030133, 0.027898, 0.025722, 0.023624, 0.021614, 0.019704,
    0.017900, 0.016205, 0.014621, 0.013148, 0.011785, 0.010530, 0.009377,
    0.008324, 0.007366, 0.006497, 0.005712, 0.005005, 0.004372, 0.003806,
    0.003302, 0.002855, 0.002460, 0.002112, 0.001806, 0.001539, 0.001307,
    0.001105, 0.000931, 0.000781, 0.000652, 0.000542, 0.000449, 0.000370,
    0.000303, 0.000247, 0.000201, 0.000162, 0.000130, 0.000104, 0.000082,
    0.000065, 0.000051, 0.000039, 0.000030, 0.000023, 0.000018, 0.000013,
    0.000010, 0.000007, 0.000005, 0.000004, 0.000003, 0.000002, 0.000001,
    0.000001, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000};
template <typename T> static const T Pa = static_cast<T>(0.01);
static const int N_Thresholds = 100;

template <typename T> static const T Min_Threshold = 0.01;
} // namespace pyin_consts
} // namespace

template <typename T>
static std::map<int, T>
probabilistic_threshold(const std::vector<T> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;

	std::map<int, T> t0_with_probability;

	T threshold = pyin_consts::Min_Threshold<T>;

	for (int n = 0; n < pyin_consts::N_Thresholds; ++n) {
		threshold += n * pyin_consts::Min_Threshold<T>;
		for (tau = 2; tau < size; tau++) {
			if (yin_buffer[tau] < threshold) {
				while (
				    tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
					tau++;
				}
				break;
			}
		}
		auto a = yin_buffer[tau] < threshold ? 1 : pyin_consts::Pa<T>;

		t0_with_probability[tau] += a * pyin_consts::Beta_Distribution[n];
	}

	return t0_with_probability;
}

template <typename T>
T
pitch_alloc::pyin(const std::vector<T> &audio_buffer, int sample_rate,
    pitch_alloc::Yin<T> *ya)
{
	difference(audio_buffer, ya);

	cumulative_mean_normalized_difference(ya->yin_buffer);

	auto tau_estimates = probabilistic_threshold(ya->yin_buffer);

	T ret;

	for (auto tau_estimate : tau_estimates) {
		ret = (tau_estimate.first != 0)
		          ? sample_rate / std::get<0>(parabolic_interpolation(
		                              ya->yin_buffer, tau_estimate.first))
		          : -1;

		std::cout << "pitch: " << ret << "\tprob: " << tau_estimate.second
		          << std::endl;
	}

	ya->clear();
	return ret;
}

template <typename T>
T
pitch::pyin(const std::vector<T> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin<T> ya(audio_buffer.size());
	return pitch_alloc::pyin(audio_buffer, sample_rate, &ya);
}

template double
pitch::pyin<double>(const std::vector<double> &audio_buffer, int sample_rate);
template float
pitch::pyin<float>(const std::vector<float> &audio_buffer, int sample_rate);

template double
pitch_alloc::pyin<double>(const std::vector<double> &audio_buffer,
    int sample_rate, pitch_alloc::Yin<double> *ya);
template float
pitch_alloc::pyin<float>(const std::vector<float> &audio_buffer,
    int sample_rate, pitch_alloc::Yin<float> *ya);

template static std::map<int, double>
probabilistic_threshold<double>(const std::vector<double> &yin_buffer);

template static std::map<int, float>
probabilistic_threshold<float>(const std::vector<float> &yin_buffer);

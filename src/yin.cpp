#include "pitch_detection/pitch_detection.h"
#include "pitch_detection_priv.h"
#include <algorithm>
#include <complex>
#include <map>
#include <tuple>
#include <vector>

namespace
{
namespace yin_consts
{
template <typename T> static const T Threshold = static_cast<T>(0.20);
}
} // namespace

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
static int
absolute_threshold(const std::vector<T> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;
	for (tau = 2; tau < size; tau++) {
		if (yin_buffer[tau] < yin_consts::Threshold<T>) {
			while (tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
				tau++;
			}
			break;
		}
	}
	return (tau == size || yin_buffer[tau] >= yin_consts::Threshold<T>) ? -1
	                                                                    : tau;
}

// pairs of (f0, probability)
template <typename T>
static std::vector<std::pair<T, T>>
probabilistic_threshold(const std::vector<T> &yin_buffer, int sample_rate)
{
	ssize_t size = yin_buffer.size();
	int tau;

	std::map<int, T> t0_with_probability;
	std::vector<std::pair<T, T>> f0_with_probability;

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

	for (auto tau_estimate : t0_with_probability) {
		auto f0 = (tau_estimate.first != 0)
		              ? sample_rate / std::get<0>(parabolic_interpolation(
		                                  yin_buffer, tau_estimate.first))
		              : -1.0;

		if (f0 != -1.0) {
			f0_with_probability.push_back(
			    std::make_pair(f0, tau_estimate.second));
		}
	}

	return f0_with_probability;
}

template <typename T>
static void
difference(const std::vector<T> &audio_buffer, pitch_alloc::Yin<T> *ya)
{
	acorr_r(audio_buffer, ya);

	for (int tau = 0; tau < ya->N / 2; tau++)
		ya->yin_buffer[tau] =
		    ya->out_real[0] + ya->out_real[1] - 2 * ya->out_real[tau];
}

template <typename T>
static void
cumulative_mean_normalized_difference(std::vector<T> &yin_buffer)
{
	double running_sum = 0.0f;

	yin_buffer[0] = 1;

	for (int tau = 1; tau < signed(yin_buffer.size()); tau++) {
		running_sum += yin_buffer[tau];
		yin_buffer[tau] *= tau / running_sum;
	}
}

template <typename T>
T
pitch_alloc::Yin<T>::pitch(const std::vector<T> &audio_buffer, int sample_rate)
{
	int tau_estimate;

	difference(audio_buffer, this);

	cumulative_mean_normalized_difference(this->yin_buffer);
	tau_estimate = absolute_threshold(this->yin_buffer);

	auto ret = (tau_estimate != -1)
	               ? sample_rate / std::get<0>(parabolic_interpolation(
	                                   this->yin_buffer, tau_estimate))
	               : -1;

	this->clear();
	return ret;
}

template <typename T>
std::vector<std::pair<T, T>>
pitch_alloc::Yin<T>::probabilistic_pitches(
    const std::vector<T> &audio_buffer, int sample_rate)
{
	difference(audio_buffer, this);

	cumulative_mean_normalized_difference(this->yin_buffer);

	auto f0_estimates = probabilistic_threshold(this->yin_buffer, sample_rate);

	this->clear();
	return f0_estimates;
}

template <typename T>
T
pitch::yin(const std::vector<T> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin<T> ya(audio_buffer.size());
	return ya.pitch(audio_buffer, sample_rate);
}

template <typename T>
std::vector<std::pair<T, T>>
pitch::pyin(const std::vector<T> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin<T> ya(audio_buffer.size());
	return ya.probabilistic_pitches(audio_buffer, sample_rate);
}

template class pitch_alloc::Yin<double>;
template class pitch_alloc::Yin<float>;

template double
pitch::yin<double>(const std::vector<double> &audio_buffer, int sample_rate);

template float
pitch::yin<float>(const std::vector<float> &audio_buffer, int sample_rate);

template std::vector<std::pair<double, double>>
pitch::pyin<double>(const std::vector<double> &audio_buffer, int sample_rate);

template std::vector<std::pair<float, float>>
pitch::pyin<float>(const std::vector<float> &audio_buffer, int sample_rate);

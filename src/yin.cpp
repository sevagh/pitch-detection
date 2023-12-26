#include "pitch_detection.h"
#include <algorithm>
#include <complex>
#include <map>
#include <tuple>
#include <vector>

#define YIN_THRESHOLD 0.20
#define PYIN_PA 0.01
#define PYIN_N_THRESHOLDS 100
#define PYIN_MIN_THRESHOLD 0.025

// Beta distribution with mean 0.1 and beta 18
// generated with /misc/generate_beta_distribution.py, first parameter set '0'
static const float Beta_Distribution_0[100] = {0.000000, 0.029069, 0.048836,
    0.061422, 0.068542, 0.071571, 0.071607, 0.069516, 0.065976, 0.061512,
    0.056523, 0.051309, 0.046089, 0.041021, 0.036211, 0.031727, 0.027608,
    0.023871, 0.020517, 0.017534, 0.014903, 0.012601, 0.010601, 0.008875,
    0.007393, 0.006130, 0.005059, 0.004155, 0.003397, 0.002765, 0.002239,
    0.001806, 0.001449, 0.001157, 0.000920, 0.000727, 0.000572, 0.000448,
    0.000349, 0.000271, 0.000209, 0.000160, 0.000122, 0.000092, 0.000070,
    0.000052, 0.000039, 0.000029, 0.000021, 0.000015, 0.000011, 0.000008,
    0.000006, 0.000004, 0.000003, 0.000002, 0.000001, 0.000001, 0.000001,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000};

template <typename T>
static int
absolute_threshold(const std::vector<T> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;
	for (tau = 2; tau < size; tau++) {
		if (yin_buffer[tau] < YIN_THRESHOLD) {
			while (tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
				tau++;
			}
			break;
		}
	}
	return (tau == size || yin_buffer[tau] >= YIN_THRESHOLD) ? -1 : tau;
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

	T threshold = 0.0f;

	for (int n = 0; n < PYIN_N_THRESHOLDS; ++n) {
		threshold = (n + 1) * PYIN_MIN_THRESHOLD;
		for (tau = 2; tau < size; tau++) {
			if (yin_buffer[tau] < threshold) {
				while (
				    tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
					tau++;
				}
				break;
			}
		}
		auto a = yin_buffer[tau] < threshold ? 1 : PYIN_PA;
		t0_with_probability[tau] += a * Beta_Distribution_0[n];
	}

	for (auto tau_estimate : t0_with_probability) {
		auto f0 = (tau_estimate.first != 0)
		              ? sample_rate / std::get<0>(util::parabolic_interpolation(
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
	util::acorr_r(audio_buffer, ya);

	for (int tau = 0; tau < ya->yin_buffer_size; tau++)
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
	               ? sample_rate / std::get<0>(util::parabolic_interpolation(
	                                   this->yin_buffer, tau_estimate))
	               : -1;

	this->clear();
	return ret;
}

template <typename T>
T
pitch_alloc::Yin<T>::probabilistic_pitch(
    const std::vector<T> &audio_buffer, int sample_rate)
{
	difference(audio_buffer, this);

	cumulative_mean_normalized_difference(this->yin_buffer);

	auto f0_estimates = probabilistic_threshold(this->yin_buffer, sample_rate);

	this->clear();
	return util::pitch_from_hmm(this->hmm, f0_estimates);
}

template <typename T>
T
pitch::yin(const std::vector<T> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin<T> ya(audio_buffer.size());
	return ya.pitch(audio_buffer, sample_rate);
}

template <typename T>
T
pitch::pyin(const std::vector<T> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin<T> ya(audio_buffer.size());
	return ya.probabilistic_pitch(audio_buffer, sample_rate);
}

template class pitch_alloc::Yin<double>;
template class pitch_alloc::Yin<float>;

template double
pitch::yin<double>(const std::vector<double> &audio_buffer, int sample_rate);

template float
pitch::yin<float>(const std::vector<float> &audio_buffer, int sample_rate);

template double
pitch::pyin<double>(const std::vector<double> &audio_buffer, int sample_rate);

template float
pitch::pyin<float>(const std::vector<float> &audio_buffer, int sample_rate);

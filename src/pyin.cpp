#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include "yin_priv.h"
#include <algorithm>
#include <complex>
#include <map>
#include <tuple>
#include <vector>
#include <iostream>

template <typename T>
static std::map<int, T>
probabilistic_threshold(const std::vector<T> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;

	std::map<int, T> t0_with_probability;

	T threshold = 0.0;
	for (int n = 0; n < PYIN_THRESHOLD_N;
	     ++n) { // use a range of 100 yin thresholds from 0.01 to unity
		threshold += 0.01;

		for (tau = 2; tau < size; tau++) {
			if (yin_buffer[tau] < threshold) {
				while (
				    tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
					tau++;
				}
				break;
			}
		}
		auto a = yin_buffer[tau] < threshold ? 1 : PYIN_Pa;

		t0_with_probability[tau] += a * threshold;
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

		std::cout << "pitch: " << ret << "\tprob: " << tau_estimate.second << std::endl;
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

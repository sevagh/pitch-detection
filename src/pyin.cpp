#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include "yin_priv.h"
#include <algorithm>
#include <complex>
#include <map>
#include <tuple>
#include <vector>

template <typename T>
static int
probabilistic_threshold(const std::vector<T> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;

	std::map<int, T> pitches_with_probability;

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
		auto iverson = (tau == size || yin_buffer[tau] >= threshold) ? 0 : 1;
		auto a = yin_buffer[tau] < threshold ? 1 : PYIN_Pa;

		pitches_with_probability[tau] += a * threshold * iverson;
	}

	auto best_tau = std::max_element(std::begin(pitches_with_probability),
	    std::end(pitches_with_probability),
	    [](const auto &p1, const auto &p2) { return p1.second < p2.second; });

	return best_tau->first;
}

template <typename T>
T
pitch_alloc::pyin(const std::vector<T> &audio_buffer, int sample_rate,
    pitch_alloc::Yin<T> *ya)
{
	int tau_estimate;

	difference(audio_buffer, ya);

	cumulative_mean_normalized_difference(ya->yin_buffer);
	tau_estimate = probabilistic_threshold(ya->yin_buffer);

	auto ret = (tau_estimate != -1)
	               ? sample_rate / std::get<0>(parabolic_interpolation(
	                                   ya->yin_buffer, tau_estimate))
	               : -1;

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

template static int
probabilistic_threshold<double>(const std::vector<double> &yin_buffer);
template static int
probabilistic_threshold<float>(const std::vector<float> &yin_buffer);

#include "pitch_detection/pitch_detection.h"
#include "pitch_detection_priv.h"
#include <algorithm>
#include <complex>
#include <float.h>
#include <map>
#include <numeric>
#include <vector>

namespace
{
namespace mpm_consts
{
template <typename T> static const T Cutoff = static_cast<T>(0.93);
template <typename T> static const T Small_Cutoff = static_cast<T>(0.5);
template <typename T> static const T Lower_Pitch_Cutoff = static_cast<T>(80.0);
} // namespace mpm_consts
} // namespace

namespace
{
namespace pmpm_consts
{

template <typename T> static const T Pa = static_cast<T>(0.01);
static const int N_Cutoffs = 20;

/*
 * Let's choose to modify Cutoff with the probabilistic model between 0.8
 * and 1.0 as suggested in the paper
 * Let's also decide that each value of cutoff has an even probability of being
 * correct i.e. probabilities[20] = {1.0/20, 1.0/20, ...}
 */
template <typename T>
static const T Probability_Distribution = static_cast<T>(0.05);

template <typename T> static const T Cutoff_Begin = static_cast<T>(0.8);
template <typename T> static const T Cutoff_Step = static_cast<T>(0.01);

template <typename T> static const T Small_Cutoff = static_cast<T>(0.5);

template <typename T> static const T Lower_Pitch_Cutoff = static_cast<T>(80.0);
} // namespace pmpm_consts
} // namespace

template <typename T>
static std::vector<int>
peak_picking(const std::vector<T> &nsdf)
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
		if (nsdf[pos] > nsdf[pos - 1] && nsdf[pos] >= nsdf[pos + 1] &&
		    (cur_max_pos == 0 || nsdf[pos] > nsdf[cur_max_pos])) {
			cur_max_pos = pos;
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

template <typename T>
std::vector<std::pair<T, T>>
pitch_alloc::Mpm<T>::probabilistic_pitches(
    const std::vector<T> &audio_buffer, int sample_rate)
{
	acorr_r(audio_buffer, this);

	std::map<T, T> t0_with_probability;
	std::vector<std::pair<T, T>> f0_with_probability;

	T cutoff = pmpm_consts::Cutoff_Begin<T>;

	for (int n = 0; n < pmpm_consts::N_Cutoffs; ++n) {
		std::vector<int> max_positions = peak_picking(this->out_real);
		std::vector<std::pair<T, T>> estimates;

		T highest_amplitude = -DBL_MAX;

		for (int i : max_positions) {
			highest_amplitude = std::max(highest_amplitude, this->out_real[i]);
			if (this->out_real[i] > pmpm_consts::Small_Cutoff<T>) {
				auto x = parabolic_interpolation(this->out_real, i);
				estimates.push_back(x);
				highest_amplitude = std::max(highest_amplitude, std::get<1>(x));
			}
		}

		if (estimates.empty())
			continue;

		T actual_cutoff = cutoff * highest_amplitude;
		T period = 0;

		for (auto i : estimates) {
			if (std::get<1>(i) >= actual_cutoff) {
				period = std::get<0>(i);
				break;
			}
		}

		auto a = period != 0 ? 1 : pmpm_consts::Pa<T>;

		t0_with_probability[period] +=
		    a * pmpm_consts::Probability_Distribution<T>;

		cutoff += pmpm_consts::Cutoff_Step<T>;
	}

	for (auto tau_estimate : t0_with_probability) {
		if (tau_estimate.first == 0.0) {
			continue;
		}
		auto f0 = (sample_rate / tau_estimate.first);

		f0 = (f0 > pmpm_consts::Lower_Pitch_Cutoff<T>) ? f0 : -1;

		if (f0 != -1.0) {
			f0_with_probability.push_back(
			    std::make_pair(f0, tau_estimate.second));
		}
	}
	this->clear();

	return f0_with_probability;
}

template <typename T>
T
pitch_alloc::Mpm<T>::pitch(const std::vector<T> &audio_buffer, int sample_rate)
{
	acorr_r(audio_buffer, this);

	std::vector<int> max_positions = peak_picking(this->out_real);
	std::vector<std::pair<T, T>> estimates;

	T highest_amplitude = -DBL_MAX;

	for (int i : max_positions) {
		highest_amplitude = std::max(highest_amplitude, this->out_real[i]);
		if (this->out_real[i] > mpm_consts::Small_Cutoff<T>) {
			auto x = parabolic_interpolation(this->out_real, i);
			estimates.push_back(x);
			highest_amplitude = std::max(highest_amplitude, std::get<1>(x));
		}
	}

	if (estimates.empty())
		return -1;

	T actual_cutoff = mpm_consts::Cutoff<T> * highest_amplitude;
	T period = 0;

	for (auto i : estimates) {
		if (std::get<1>(i) >= actual_cutoff) {
			period = std::get<0>(i);
			break;
		}
	}

	T pitch_estimate = (sample_rate / period);

	this->clear();

	return (pitch_estimate > mpm_consts::Lower_Pitch_Cutoff<T>) ? pitch_estimate
	                                                            : -1;
}

template <typename T>
T
pitch::mpm(const std::vector<T> &audio_buffer, int sample_rate)
{
	pitch_alloc::Mpm<T> ma(audio_buffer.size());
	return ma.pitch(audio_buffer, sample_rate);
}

template <typename T>
std::vector<std::pair<T, T>>
pitch::pmpm(const std::vector<T> &audio_buffer, int sample_rate)
{
	pitch_alloc::Mpm<T> ma(audio_buffer.size());
	return ma.probabilistic_pitches(audio_buffer, sample_rate);
}

template class pitch_alloc::Mpm<double>;
template class pitch_alloc::Mpm<float>;

template double
pitch::mpm<double>(const std::vector<double> &audio_buffer, int sample_rate);

template float
pitch::mpm<float>(const std::vector<float> &audio_buffer, int sample_rate);

template std::vector<std::pair<double, double>>
pitch::pmpm<double>(const std::vector<double> &audio_buffer, int sample_rate);

template std::vector<std::pair<float, float>>
pitch::pmpm<float>(const std::vector<float> &audio_buffer, int sample_rate);

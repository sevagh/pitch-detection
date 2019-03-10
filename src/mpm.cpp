#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <algorithm>
#include <complex>
#include <float.h>
#include <numeric>
#include <vector>

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
T
pitch_alloc::mpm(const std::vector<T> &audio_buffer, int sample_rate,
    pitch_alloc::Mpm<T> *ma)
{
	acorr_r(audio_buffer, ma);

	std::vector<int> max_positions = peak_picking(ma->out_real);
	std::vector<std::pair<T, T>> estimates;

	T highest_amplitude = -DBL_MAX;

	for (int i : max_positions) {
		highest_amplitude = std::max(highest_amplitude, ma->out_real[i]);
		if (ma->out_real[i] > MPM_SMALL_CUTOFF) {
			auto x = parabolic_interpolation(ma->out_real, i);
			estimates.push_back(x);
			highest_amplitude = std::max(highest_amplitude, std::get<1>(x));
		}
	}

	if (estimates.empty())
		return -1;

	T actual_cutoff = MPM_CUTOFF * highest_amplitude;
	T period = 0;

	for (auto i : estimates) {
		if (std::get<1>(i) >= actual_cutoff) {
			period = std::get<0>(i);
			break;
		}
	}

	T pitch_estimate = (sample_rate / period);

	ma->clear();

	return (pitch_estimate > MPM_LOWER_PITCH_CUTOFF) ? pitch_estimate : -1;
}

template <typename T>
T
pitch::mpm(const std::vector<T> &audio_buffer, int sample_rate)
{
	pitch_alloc::Mpm<T> ma(audio_buffer.size());
	return pitch_alloc::mpm(audio_buffer, sample_rate, &ma);
}

template class pitch_alloc::Mpm<double>;
template class pitch_alloc::Mpm<float>;

template double
pitch::mpm<double>(const std::vector<double> &audio_buffer, int sample_rate);
template double
pitch_alloc::mpm<double>(const std::vector<double> &audio_buffer,
    int sample_rate, pitch_alloc::Mpm<double> *ma);

template float
pitch::mpm<float>(const std::vector<float> &audio_buffer, int sample_rate);
template float
pitch_alloc::mpm<float>(const std::vector<float> &audio_buffer, int sample_rate,
    pitch_alloc::Mpm<float> *ma);

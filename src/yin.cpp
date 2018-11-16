#include <cstdlib>
#include <iostream>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <tuple>
#include <vector>

static int
absolute_threshold(const std::vector<double> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;
	for (tau = 2; tau < size; tau++) {
		if (yin_buffer[tau] < YIN_DEFAULT_THRESHOLD) {
			while (tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
				tau++;
			}
			break;
		}
	}
	return (tau == size || yin_buffer[tau] >= YIN_DEFAULT_THRESHOLD) ? -1 : tau;
}

static std::vector<double>
difference(const std::vector<double> &data)
{
	int index, tau;
	double delta;
	int yin_buffer_size = signed(data.size() / 2);
	std::vector<double> yin_buffer(yin_buffer_size, 0.0);
	for (tau = 0; tau < yin_buffer_size; tau++) {
		yin_buffer[tau] = 0;
	}
	for (tau = 1; tau < yin_buffer_size; tau++) {
		for (index = 0; index < yin_buffer_size; index++) {
			delta = data[index] - data[index + tau];
			yin_buffer[tau] += delta * delta;
		}
	}
	return yin_buffer;
}

static void
cumulative_mean_normalized_difference(std::vector<double> &yin_buffer)
{
	int tau;
	yin_buffer[0] = 1;
	double running_sum = 0;
	int yin_buffer_size = signed(yin_buffer.size());
	for (tau = 1; tau < yin_buffer_size; tau++) {
		running_sum += yin_buffer[tau];
		yin_buffer[tau] *= tau / running_sum;
	}
}

double
pitch::yin(const std::vector<double> &audio_buffer, int sample_rate)
{
	int tau_estimate;

	std::vector<double> yin_buffer = difference(audio_buffer);
	cumulative_mean_normalized_difference(yin_buffer);
	tau_estimate = absolute_threshold(yin_buffer);

	return (tau_estimate != -1)
	           ? sample_rate / std::get<0>(parabolic_interpolation(
	                               yin_buffer, tau_estimate))
	           : -1;
}

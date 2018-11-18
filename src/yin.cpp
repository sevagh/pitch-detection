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
	std::vector<double> acorr = acorr_r(data);

	std::vector<double> difference;
	difference.reserve(acorr.size() / 2);

	for (int tau = 0; tau < signed(difference.capacity()); tau++)
		difference.push_back(2 * acorr[0] - 2 * acorr[tau]);

	return difference;
}

static void
cumulative_mean_normalized_difference(std::vector<double> &yin_buffer)
{
	double running_sum = 0.0f;

	yin_buffer[0] = 1;

	for (int tau = 1; tau < signed(yin_buffer.size()); tau++) {
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

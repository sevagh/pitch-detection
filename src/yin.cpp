#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <complex>
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

static void
difference(const std::vector<double> &audio_buffer, pitch_alloc::Yin *ya)
{
	acorr_r(audio_buffer, ya);

	for (int tau = 0; tau < ya->N / 2; tau++)
		ya->yin_buffer[tau] = 2 * ya->out_real[0] - 2 * ya->out_real[tau];
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
pitch_alloc::yin(const std::vector<double> &audio_buffer, int sample_rate,
    pitch_alloc::Yin *ya)
{
	int tau_estimate;

	difference(audio_buffer, ya);

	cumulative_mean_normalized_difference(ya->yin_buffer);
	tau_estimate = absolute_threshold(ya->yin_buffer);

	auto ret = (tau_estimate != -1)
	               ? sample_rate / std::get<0>(parabolic_interpolation(
	                                   ya->yin_buffer, tau_estimate))
	               : -1;

	ya->clear();
	return ret;
}

double
pitch::yin(const std::vector<double> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin ya(audio_buffer.size());
	return pitch_alloc::yin(audio_buffer, sample_rate, &ya);
}

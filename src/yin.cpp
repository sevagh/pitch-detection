#include <vector>
#include <iostream>
#include <tuple>
#include <cstdlib>
#include "yin.h"
#include "mpm_yin_common.h"

#define DEFAULT_THRESHOLD 0.20
#define DEFAULT_OVERLAP 1536

static int absolute_threshold(std::vector<double> yin_buffer)
{
    ssize_t size = yin_buffer.size();
	int tau;
	for (tau = 2; tau < size; tau++) {
		if (yin_buffer[tau] < DEFAULT_THRESHOLD) {
			while (tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
				tau++;
			}
			break;
		}
	}
	return (tau == size || yin_buffer[tau] >= DEFAULT_THRESHOLD) ? -1 : tau;
}

double get_pitch_yin(std::vector<double> audio_buffer, int sample_rate)
{
	int tau_estimate;

    std::vector<double> yin_buffer = nsdf_time_domain(audio_buffer);
	tau_estimate = absolute_threshold(yin_buffer);

    auto x = parabolic_interpolation(yin_buffer[tau_estimate-1], yin_buffer[tau_estimate], yin_buffer[tau_estimate+1], tau_estimate);

    return (tau_estimate != -1) ? sample_rate / std::get<0>(x) / 2 : -1;
}

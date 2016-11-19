#include <float.h>
#include "yin.h"

#define DEFAULT_THRESHOLD 0.20
#define DEFAULT_OVERLAP 1536

yin::yin()
{
}

void yin::init(double sampling_rate, int size)
{
	yin::sampling_rate = sampling_rate;
	yin::data_size = size;

	yin::yin_buffer_size = (int) size/2;
	yin::yin_buffer = new double[yin::yin_buffer_size];
}

void yin::cleanup()
{
	delete[] yin_buffer;
}

double yin::parabolic_interpolation(int tau_estimate)
{
	float betterTau;
	int x0;
	int x2;

	if (tau_estimate < 1) {
		x0 = tau_estimate;
	} else {
		x0 = tau_estimate - 1;
	}
	if (tau_estimate + 1 < yin_buffer_size) {
		x2 = tau_estimate + 1;
	} else {
		x2 = tau_estimate;
	}
	if (x0 == tau_estimate) {
		if (yin_buffer[tau_estimate] <= yin_buffer[x2]) {
			betterTau = tau_estimate;
		} else {
			betterTau = x2;
		}
	} else if (x2 == tau_estimate) {
		if (yin_buffer[tau_estimate] <= yin_buffer[x0]) {
			betterTau = tau_estimate;
		} else {
			betterTau = x0;
		}
	} else {
		float s0, s1, s2;
		s0 = yin_buffer[x0];
		s1 = yin_buffer[tau_estimate];
		s2 = yin_buffer[x2];
		betterTau = tau_estimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
	}
	return betterTau;
}

void yin::difference(double *data)
{
	int index, tau;
	float delta;
	for (tau = 0; tau < yin_buffer_size; tau++) {
		yin_buffer[tau] = 0;
	}
	for (tau = 1; tau < yin_buffer_size; tau++) {
		for (index = 0; index < yin_buffer_size; index++) {
			delta = data[index] - data[index + tau];
			yin_buffer[tau] += delta * delta;
		}
	}
}

void yin::cumulative_mean_normalized_difference()
{
	int tau;
	yin_buffer[0] = 1;
	float runningSum = 0;
	for (tau = 1; tau < yin_buffer_size; tau++) {
		runningSum += yin_buffer[tau];
		yin_buffer[tau] *= tau / runningSum;
	}
}

int yin::absolute_threshold()
{
	int tau;
	for (tau = 2; tau < yin_buffer_size; tau++) {
		if (yin_buffer[tau] < DEFAULT_THRESHOLD) {
			while (tau + 1 < yin_buffer_size && yin_buffer[tau + 1] < yin_buffer[tau]) {
				tau++;
			}
			break;
		}
	}

	if (tau == yin_buffer_size || yin_buffer[tau] >= DEFAULT_THRESHOLD) {
		tau = -1;
	}

	return tau;
}

double yin::get_pitch(double *data)
{
	int tau_estimate;
	double pitch;

	yin::difference(data);
	yin::cumulative_mean_normalized_difference();
	tau_estimate = yin::absolute_threshold();
	if (tau_estimate != -1) {
		float betterTau = parabolic_interpolation(tau_estimate);
		pitch = sampling_rate / betterTau;
	} else{
		pitch = -1;
	}
	
	return pitch;
}


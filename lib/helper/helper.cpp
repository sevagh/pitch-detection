//
// Created by sevag on 4/29/15.
//

#include "helper.h"
#include "math.h"

#define FREQ_MIN 80
#define FREQ_MAX 10000

void hanning_window(double *signal, int size) {
    for (int i = 0; i < size; i++) {
        int j = i; // j = index into Hann window function
        signal[i] = (signal[i] * 0.5 * (1.0 - cos(2.0 * M_PI * j / size)));
    }
}

double *zero_pad(double *original_signal, int size) {
    double *new_signal = new double[2*size];
    for (int i = 0; i < size; i++) {
        new_signal[i] = original_signal[i];
    }
    return new_signal;
}

double get_snr(double frequency, double *arr,
	       int N, double sampling_rate, double (*fp) (double, double*, int, double))
{
	double snr;

	int index;

	double total;
	double E;

	total = 0;
	for (index = 0; index < N; index++)
		total += arr[index] * arr[index];

	E = fp(frequency, arr, N, sampling_rate);

	snr = 10.0f * log10((double) E / (double) (fabs(total - E)));

	return snr;
}


double looper(double (*fp)(double, double*, int, double,
			   double (*fp)(double, double*, int, double)),
	      double *data, int data_size, double sampling_rate, double (*fp2)(double, double*, int, double))
{
	double freq_best = 0.0f;
	double snr_best = -1000.0f;

	double freq_increment = 1000;
	double freq_min = FREQ_MIN;
	double freq_max = FREQ_MAX;
	double freq = FREQ_MIN;

	while (freq_increment >= 0.1) {
		for (double freq = freq_min; freq <= freq_max; freq += freq_increment) {
			double snr_current = fp(freq, data, data_size, sampling_rate, fp2);
			if (snr_current > snr_best) {
				snr_best = snr_current;
				freq_best = freq;
			}
		}
		freq_min = freq_best - freq_increment;
		freq_max = freq_best + freq_increment;
		freq_increment = freq_increment/10;
	}

	return freq_max;
}

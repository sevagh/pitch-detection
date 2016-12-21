#ifndef LOOP_H
#define LOOP_H

#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>

#define FREQ_MIN 0
#define FREQ_MAX 10000
#define CONSEC_FAIL_LIM 2
#define CONSEC_FAIL_MARGIN 0.01
#define FREQ_STARTING_INCR 10

static double get_snr(double frequency, std::vector<double> arr,
		      int N, int sampling_rate, double (*fp) (double, std::vector<double>, int, int))
{
	double snr;

	int index;

	double total;
	double E;

	total = 0;
	for (index = 0; index < N; index++)
		total += arr[index] * arr[index];

	E = fp(frequency, arr, N, sampling_rate);

	snr = 10.0 * log10((double) E / (double) (fabs(total - E)));

	return snr;
}


inline double loop(std::vector<double> data, int data_size, int sample_rate,
		   double (*fp)(double, std::vector<double>, int, int))
{
	double freq_best = 0.0;
	double snr_max_loc = -999.0;
	double snr_max_glob = -1000.0;
	double freq_incr = FREQ_STARTING_INCR;
	double freq_min = FREQ_MIN;
	double freq_max = FREQ_MAX;
	int consec_fail = 0;

	while ((snr_max_loc >= snr_max_glob) && (consec_fail < CONSEC_FAIL_LIM)) {
		for (double freq = freq_min; freq <= freq_max;
		     freq += freq_incr) {
			double snr_current = get_snr(freq,
						     data, data_size,
						     sample_rate, fp);
			if (snr_current > snr_max_loc) {
				snr_max_loc = snr_current;
				freq_best = freq;
			}
		}
		if (abs(snr_max_loc-snr_max_glob) <= CONSEC_FAIL_MARGIN) {
			consec_fail++;
		}
		if (snr_max_loc > snr_max_glob) {
			snr_max_glob = snr_max_loc;
			freq_incr /= 10.0;
			freq_min = std::max(0.0, freq_best-freq_incr);
			freq_max = freq_best+freq_incr;
		}
	}

	return freq_best;
}

#endif /* LOOP_H */

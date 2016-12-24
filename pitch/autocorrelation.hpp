#ifndef AUTOCORRELATION_H
#define AUTOCORRELATION_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <autocovariance.hpp>

static double get_acf_periodicity(std::vector<double> data, int size)
{
	int starting_index = 0;
	int total_peak_bin_index = 0;
	int occurences = 0;

	for (int i = 1; i < size-1; i++) {
		if ((data[i] > data[i-1]) && (data[i]) > data[i+1]) {
			occurences += 1;
			total_peak_bin_index +=
				(i - starting_index);
			starting_index = i;
		}
	}

	return (double) (total_peak_bin_index) / (double) occurences;
}

double get_pitch_autocorrelation(std::vector<double> data, int sample_rate)
{
	int size = signed(data.size());
	std::vector<double> acf = autocovariance(data);

	double peak_bin_index_periodicity = get_acf_periodicity(acf, size);

	return (sample_rate/peak_bin_index_periodicity);
}

#endif /* AUTOCORRELATION_H */

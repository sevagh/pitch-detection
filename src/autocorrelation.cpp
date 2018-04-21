#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <vector>

static double
get_acf_periodicity(const std::vector<double> &data)
{
	int size = data.size();
	int total_peak_bin_index = 0;
	int occurences = 0;

	for (int i = 1; i < size - 1; i++) {
		if ((data[i] > data[i - 1]) && (data[i]) > data[i + 1]) {
			occurences += 1;
			total_peak_bin_index = i;
		}
	}

	return (double)(total_peak_bin_index) / (double)occurences;
}

double
get_pitch_autocorrelation(const std::vector<double> &data, int sample_rate)
{
	double peak_bin_index_periodicity = get_acf_periodicity(acorr_r(data));

	return (sample_rate / peak_bin_index_periodicity);
}

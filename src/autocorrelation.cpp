#include <algorithm>
#include <cmath>
#include <complex>
#include <numeric>
#include <pitch_detection.h>
#include <pitch_detection_priv.h>
#include <vector>
#include <iostream>
extern "C" {
#include <complex.h>
#include <xcorr.h>
}

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
	int size = data.size();
	int size2 = 2 * size - 1;
	std::vector<std::complex<double>> acf_complex(size2);
	xcorr_fftw_r2c(data.data(), data.data(), acf_complex.data(), size);
	std::vector<double> acf_real(size2);

	for (int i = 0; i < size2; ++i)
		acf_real[i] = acf_complex[i].real();

	double peak_bin_index_periodicity = get_acf_periodicity(acf_real);

	return (sample_rate / peak_bin_index_periodicity);
}

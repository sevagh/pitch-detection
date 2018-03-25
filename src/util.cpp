#include "pitch_detection_priv.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

void
hanning_window(std::vector<double> &signal)
{
	std::transform(signal.begin(), signal.end(), signal.begin(),
	    [ sz = (double)signal.size(), idx = 0 ](double n) mutable {
		    double ret = n * 0.5 * (1.0 - cos(2.0 * M_PI * idx / sz));
		    ++idx;
		    return ret;
	    });
}

void
zero_pad(std::vector<double> &original_signal)
{
	original_signal.resize(2 * original_signal.size(), 0);
}

static double
get_snr(double frequency, const std::vector<double> &data, double sampling_rate,
    double (*fp)(double, const std::vector<double> &, double))
{
	double snr;

	double total;
	double E;

	total = 0;
	for (uint index = 0; index < data.size(); ++index)
		total += data[index] * data[index];

	E = fp(frequency, data, sampling_rate);

	snr = 10.0 * log10((double)E / (double)(fabs(total - E)));

	return snr;
}

double
looper(const std::vector<double> &data, int sampling_rate,
    double (*fp)(double, const std::vector<double> &, double))
{
	double freq_best = 0.0;
	double snr_max_loc = -999.0;

	for (double freq = 25.0; freq < 4200.0; freq += 0.1) {
		double snr_current = get_snr(freq, data, sampling_rate, fp);
		if (snr_current > snr_max_loc) {
			snr_max_loc = snr_current;
			freq_best = freq;
		}
	}
	return freq_best;
}

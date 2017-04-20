#ifndef AUTOCORRELATION_H
#define AUTOCORRELATION_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <complex>

extern "C" {
#include <xcorr.h>
}

double get_pitch_autocorrelation(std::vector<double> data, int sample_rate);

#endif /* AUTOCORRELATION_H */

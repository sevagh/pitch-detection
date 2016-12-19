#include <float.h>
#include <algorithm>
#include <vector>
#include <tuple>
#include <numeric>
#include "mpm.h"

std::tuple<double, double> parabolic_interpolation(double nsdfa, double nsdfb, double nsdfc, int tau);

std::vector<double> nsdf_time_domain(std::vector<double> audio_buffer);

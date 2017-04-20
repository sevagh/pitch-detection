#ifndef MPM_H
#define MPM_H

#include <float.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <float.h>
#include <iostream>
#include <numeric>
#include <complex>
#include "parabolic_interpolation.hpp"

#define CUTOFF 0.93 //0.97 is default
#define SMALL_CUTOFF 0.5
#define LOWER_PITCH_CUTOFF 80 //hz

extern "C" {
#include <xcorr.h>
}

double get_pitch_mpm(std::vector<double> audio_buffer, int sample_rate);

#endif /* MPM_H */

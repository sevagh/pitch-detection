#ifndef YIN_H
#define YIN_H

#include <vector>
#include <iostream>
#include <tuple>
#include <cstdlib>
#include <parabolic_interpolation.hpp>

#define DEFAULT_THRESHOLD 0.20
#define DEFAULT_OVERLAP 1536

double get_pitch_yin(std::vector<double> audio_buffer, int sample_rate);

#endif /* YIN_H */

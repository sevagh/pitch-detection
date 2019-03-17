#ifndef PITCH_DETECTION_PRIV_H
#define PITCH_DETECTION_PRIV_H

#include "pitch_detection.h"
#include <complex>
#include <vector>

template <typename T>
std::pair<T, T>
parabolic_interpolation(const std::vector<T> &, int);

template <typename T>
void
acorr_r(const std::vector<T> &, pitch_alloc::Mpm<T> *);

template <typename T>
void
cumulative_mean_normalized_difference(std::vector<T> &);

template <typename T>
void
difference(const std::vector<T> &, pitch_alloc::Yin<T> *);

#endif /* PITCH_DETECTION_PRIV_H */

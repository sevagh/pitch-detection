#ifndef PITCH_DETECTION_PRIV_H
#define PITCH_DETECTION_PRIV_H

#include "pitch_detection/pitch_detection.h"
#include <complex>
#include <vector>

template <typename T>
std::pair<T, T>
parabolic_interpolation(const std::vector<T> &, int);

template <typename T>
void
acorr_r(const std::vector<T> &, pitch_alloc::BaseAlloc<T> *);

#endif /* PITCH_DETECTION_PRIV_H */

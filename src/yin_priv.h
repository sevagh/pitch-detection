#ifndef YIN_PRIV_H
#define YIN_PRIV_H

/* YIN configs */
#define YIN_DEFAULT_THRESHOLD 0.20

/* PYIN configs */
#define PYIN_Pa 0.01
#define PYIN_THRESHOLD_N 100

#include "pitch_detection.h"

template <typename T>
void
cumulative_mean_normalized_difference(std::vector<T> &);

template <typename T>
void
difference(const std::vector<T> &, pitch_alloc::Yin<T> *);

#endif /* YIN_PRIV_H */

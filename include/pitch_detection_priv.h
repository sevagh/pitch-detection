#ifndef PITCH_DETECTION_PRIV_H
#define PITCH_DETECTION_PRIV_H

/* MPM configs */
#define MPM_CUTOFF 0.93
#define MPM_SMALL_CUTOFF 0.5
#define MPM_LOWER_PITCH_CUTOFF 80

/* YIN configs */
#define YIN_DEFAULT_THRESHOLD 0.20

#include <complex>
#include <vector>

std::pair<double, double>
parabolic_interpolation(const std::vector<double> &array, double x);

std::vector<double>
acorr_r(const std::vector<double> &signal);

#endif /* PITCH_DETECTION__PRIV_H */

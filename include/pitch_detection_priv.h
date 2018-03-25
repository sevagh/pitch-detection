#ifndef PITCH_DETECTION_PRIV_H
#define PITCH_DETECTION_PRIV_H

/* MPM configs */
#define MPM_CUTOFF 0.93
#define MPM_SMALL_CUTOFF 0.5
#define MPM_LOWER_PITCH_CUTOFF 80

/* YIN configs */
#define YIN_DEFAULT_THRESHOLD 0.20
#define YIN_DEFAULT_OVERLAP 1536

/* Goertzel/DFT configs */
#define FREQ_MIN 0
#define FREQ_MAX 10000
#define CONSEC_FAIL_LIM 2
#define CONSEC_FAIL_MARGIN 0.01
#define FREQ_STARTING_INCR 10

#include <complex>
#include <vector>

std::pair<double, double>
parabolic_interpolation(const std::vector<double> &array, double x);

void
zero_pad(std::vector<double> &original_signal);

void
hanning_window(std::vector<double> &signal);

double
looper(const std::vector<double> &data, int sampling_rate,
    double (*fp)(double, const std::vector<double> &, double));

#endif /* PITCH_DETECTION__PRIV_H */

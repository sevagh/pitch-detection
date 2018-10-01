#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <vector>

double
get_pitch_autocorrelation(const std::vector<double> &data, int sample_rate);

double
get_pitch_yin(const std::vector<double> &audio_buffer, int sample_rate);

double
get_pitch_mpm(const std::vector<double> &audio_buffer, int sample_rate);

double
get_pitch_goertzel(const std::vector<double> &audio_buffer, int sample_rate);

double
get_pitch_dft(const std::vector<double> &audio_buffer, int sample_rate);

extern "C" {
double
get_pitch_mpm_c(const double *arr, int size, int sample_rate);
}

#endif /* PITCH_DETECTION_H */

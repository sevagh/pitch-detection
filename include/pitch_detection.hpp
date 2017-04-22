#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <vector>

double get_pitch_autocorrelation(std::vector<double> data, int sample_rate);
double get_pitch_yin(std::vector<double> audio_buffer, int sample_rate);
double get_pitch_mpm(std::vector<double> audio_buffer, int sample_rate);

#endif /* PITCH_DETECTION_H */

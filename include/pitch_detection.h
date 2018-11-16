#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <vector>

namespace pitch
{
double
autocorrelation(const std::vector<double> &data, int sample_rate);

double
yin(const std::vector<double> &audio_buffer, int sample_rate);

double
mpm(const std::vector<double> &audio_buffer, int sample_rate);
} // namespace pitch

#endif /* PITCH_DETECTION_H */

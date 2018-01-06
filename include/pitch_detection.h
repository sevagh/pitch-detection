#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <functional>
#include <map>
#include <vector>

double
get_pitch_autocorrelation(const std::vector<double> &data, int sample_rate);

double
get_pitch_yin(const std::vector<double> &audio_buffer, int sample_rate);

double
get_pitch_mpm(const std::vector<double> &audio_buffer, int sample_rate);

enum pitch_algorithm_type {
	mpm,
	autocorrelation,
	yin,
};

using pitch_algorithm = std::function<double(std::vector<double> &, int)>;

inline std::map<pitch_algorithm_type, pitch_algorithm> pitch_algorithms = {
    {mpm, get_pitch_mpm}, {yin, get_pitch_yin},
    {autocorrelation, get_pitch_autocorrelation},
};

inline std::map<std::string, pitch_algorithm_type> pitch_types = {
    {"mpm", mpm}, {"autocorrelation", autocorrelation}, {"yin", yin},
};

#endif /* PITCH_DETECTION_H */

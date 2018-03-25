#ifndef STDIN_H
#define STDIN_H

#include <functional>
#include <map>
#include <pitch_detection.h>
#include <vector>

enum pitch_algorithm_type {
	mpm,
	autocorrelation,
	yin,
	goertzel,
	dft,
};

using pitch_algorithm = std::function<double(std::vector<double> &, int)>;

inline std::map<pitch_algorithm_type, pitch_algorithm> pitch_algorithms = {
    {mpm, get_pitch_mpm},
    {yin, get_pitch_yin},
    {goertzel, get_pitch_goertzel},
    {dft, get_pitch_dft},
    {autocorrelation, get_pitch_autocorrelation},
};

inline std::map<std::string, pitch_algorithm_type> pitch_types = {
    {"mpm", mpm},
    {"autocorrelation", autocorrelation},
    {"yin", yin},
    {"dft", dft},
    {"goertzel", goertzel},
};

#endif /* STDIN_H */

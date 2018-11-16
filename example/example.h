#ifndef STDIN_H
#define STDIN_H

#include <functional>
#include <map>
#include <pitch_detection.h>
#include <vector>

using pitch_func = std::function<double(std::vector<double> &, int)>;

inline std::map<std::string, pitch_func> pitch_funcs = {
    {"mpm", pitch::mpm},
    {"yin", pitch::yin},
    {"autocorrelation", pitch::autocorrelation},
};

#endif /* STDIN_H */

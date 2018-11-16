#ifndef UTIL_H
#define UTIL_H

#include <vector>

namespace util
{
std::tuple<std::string, double>
closest_note(double);
std::vector<double>
generate_sinewave(size_t size, double frequency, int sample_rate);
} // namespace util

#endif /* UTIL_H */

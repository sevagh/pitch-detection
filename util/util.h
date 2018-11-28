#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace util
{

std::tuple<std::string, double>
closest_note(double);

std::vector<double>
sinewave(size_t size, double frequency, int sample_rate);

} // namespace util

#endif /* UTIL_H */

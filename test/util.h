#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <complex>
#include <string>
#include <vector>

namespace test_util
{

std::vector<float>
sinewave(size_t, float, int);

std::vector<float> vec_from_file(std::string);

} // namespace test_util

#endif /* UTIL_H */

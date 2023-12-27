#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <complex>
#include <string>
#include <vector>

namespace test_util
{

template <typename T>
std::vector<T>
sinewave(size_t, T, int);

template <typename T>
std::vector<T> vec_from_file(std::string);

} // namespace test_util

#endif /* UTIL_H */

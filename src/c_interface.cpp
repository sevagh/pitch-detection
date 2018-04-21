#include <pitch_detection.h>
#include <vector>

extern "C" {
double
get_pitch_mpm_c(const double *arr, int size, int sample_rate)
{
	const std::vector<double> vec(arr, arr + size);
	return get_pitch_mpm(vec, sample_rate);
}
}

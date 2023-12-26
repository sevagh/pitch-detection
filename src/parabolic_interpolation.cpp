#include "pitch_detection.h"
#include <vector>

std::pair<float, float>
util::parabolic_interpolation(const std::vector<float> &array, int x_)
{
	int x_adjusted;
	float x = (float)x_;

	if (x < 1) {
		x_adjusted = (array[x] <= array[x + 1]) ? x : x + 1;
	} else if (x > signed(array.size()) - 1) {
		x_adjusted = (array[x] <= array[x - 1]) ? x : x - 1;
	} else {
		float den = array[x + 1] + array[x - 1] - 2 * array[x];
		float delta = array[x - 1] - array[x + 1];
		return (!den) ? std::make_pair(x, array[x])
		              : std::make_pair(x + delta / (2 * den),
		                    array[x] - delta * delta / (8 * den));
	}
	return std::make_pair(x_adjusted, array[x_adjusted]);
}

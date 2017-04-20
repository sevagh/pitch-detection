#ifndef PARABOLIC_INTERPOLATION_H
#define PARABOLIC_INTERPOLATION_H

#include <vector>

inline std::pair<double, double> parabolic_interpolation(std::vector<double> array, double x)
{
	int x_adjusted;

	if (x < 1) {
		x_adjusted = (array[x] <= array[x+1]) ? x : x+1;
	} else if (x > signed(array.size())-1) {
		x_adjusted = (array[x] <= array[x-1]) ? x : x-1;
	} else {
		double den = array[x+1] + array[x-1] - 2 * array[x];
		double delta = array[x-1] - array[x+1];
		return (!den) ? std::make_pair(x, array[x]) : std::make_pair(x + delta / (2 * den), array[x] - delta*delta/(8*den));
	}
	return std::make_pair(x_adjusted, array[x_adjusted]);
}

#endif /* PARABOLIC_INTERPOLATION_H */

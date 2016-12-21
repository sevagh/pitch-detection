#ifndef PARABOLIC_INTERPOLATION_H
#define PARABOLIC_INTERPOLATION_H

#include <vector>

inline std::pair<double, double> parabolic_interpolation(std::vector<double> array, double x)
{
	int x_;
	int x0;
	int x2;

	x0 = (x < 1) ? x : x - 1;
	x2 = (x + 1 < signed(array.size())) ? x + 1 : x;

	if (x0 == x) {
		x_ = (array[x] <= array[x2]) ? x : x2;
	} else if (x2 == x) {
		x_ = (array[x] <= array[x0]) ? x : x0;
	} else {
		double bottom = array[x+1] + array[x-1] - 2 * array[x];
		double delta = array[x-1] - array[x+1];
		return (!bottom) ? std::make_pair(x, array[x]) : std::make_pair(x + delta / (2 * bottom), array[x] - delta*delta/(8*bottom));
	}
	return std::make_pair(x_, array[x_]);
}

#endif /* PARABOLIC_INTERPOLATION_H */

#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>
#include "helper.hpp"

void hanning_window(std::vector<double>& signal)
{
	ssize_t size = signal.size();
	for (int i = 0; i < size; i++) {
		int j = i; // j = index into Hann window function
		signal[i] = (signal[i] * 0.5 * (1.0 - cos(2.0 * M_PI * j / size)));
	}
}

void zero_pad(std::vector<double>& signal)
{
	signal.resize(2*signal.size(), 0);
}

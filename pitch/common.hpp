#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <complex>

std::vector<std::complex<double>> get_complex_from_real(std::vector<double> data)
{
	std::vector<std::complex<double>> complex_data{};
	
	for (double datum : data)
		complex_data.push_back({datum, 0});

	return complex_data;
}

#endif /* COMMON_H */

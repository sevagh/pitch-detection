#include <float.h>
#include <algorithm>
#include <vector>
#include <tuple>
#include <numeric>

std::tuple<double, double> parabolic_interpolation(double nsdfa, double nsdfb, double nsdfc, int tau)
{
	double bValue = tau;
	double bottom = nsdfc + nsdfa - 2 * nsdfb;
	double delta = nsdfa - nsdfc;
	return (!bottom) ? std::make_tuple(bValue, nsdfb) : std::make_tuple(bValue + delta / (2 * bottom), nsdfb - delta*delta/(8*bottom));
}

std::vector<double> nsdf_time_domain(std::vector<double> audio_buffer)
{
	std::vector<double> nsdf{};
	ssize_t size = audio_buffer.size();
	for (int tau = 0; tau < size; tau++) {
		double acf = 0;
		double divisorM = 0;
		for (int i = 0; i < size - tau; i++) {
			acf += audio_buffer[i] * audio_buffer[i + tau];
			divisorM += audio_buffer[i] * audio_buffer[i] + audio_buffer[i + tau] * audio_buffer[i + tau];
		}
		nsdf.push_back(2 * acf / divisorM);
	}
	return nsdf;
}

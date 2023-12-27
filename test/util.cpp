#include "util.h"
#include "pitch_detection.h"
#include <cmath>
#include <float.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::vector<T>
test_util::sinewave(size_t size, T frequency, int sample_rate)
{
	std::vector<T> tone_single_channel(size / 2);

	T delta_phi = 2.0 * M_PI * frequency / sample_rate;
	T phase = 0.0;

	for (size_t i = 0; i < size / 2; ++i) {
		tone_single_channel[i] = sin(phase);
		phase += delta_phi;
	}

	return tone_single_channel;
}

template <typename T>
std::vector<T>
test_util::vec_from_file(std::string path)
{
	std::vector<T> data;
	std::ifstream infile(path);

	if (infile.fail()) {
		std::cerr << "File '" << path << "' doesn't exist, exiting"
		          << std::endl;
		exit(1);
	}

	T val;
	while (infile >> val)
		data.push_back(val);

	return data;
}

// create float, double template specializations
template std::vector<float>
test_util::sinewave<float>(size_t size, float frequency, int sample_rate);

template std::vector<double>
test_util::sinewave<double>(size_t size, double frequency, int sample_rate);

template std::vector<float>
test_util::vec_from_file<float>(std::string path);

template std::vector<double>
test_util::vec_from_file<double>(std::string path);

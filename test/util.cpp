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

std::vector<float>
test_util::sinewave(size_t size, float frequency, int sample_rate)
{
	std::vector<float> tone_single_channel(size / 2);

	float delta_phi = 2.0 * M_PI * frequency / sample_rate;
	float phase = 0.0;

	for (size_t i = 0; i < size / 2; ++i) {
		tone_single_channel[i] = sin(phase);
		phase += delta_phi;
	}

	return tone_single_channel;
}

std::vector<float>
test_util::vec_from_file(std::string path)
{
	std::vector<float> data;
	std::ifstream infile(path);

	if (infile.fail()) {
		std::cerr << "File '" << path << "' doesn't exist, exiting"
		          << std::endl;
		exit(1);
	}

	float val;
	while (infile >> val)
		data.push_back(val);

	return data;
}

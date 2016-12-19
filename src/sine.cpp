#include <stdlib.h>
#include <vector>
#include <tuple>
#include "sine.h"
#include "math.h"
#include "pitch.h"

#define SIZE 4096

std::tuple<std::vector<double>, std::vector<double>> generate_sine(double frequency, int sample_rate)
{
	int LUT_SIZE = SIZE / 4;
    int SINE_SIZE = SIZE / 2;

    std::vector<double> tone_single_channel(SINE_SIZE, 0.0);
    std::vector<double> tone_dual_channel(SIZE, 0.0);

	double doublef = (double) frequency;
	double delta_phi = doublef * LUT_SIZE * 1.0 / sample_rate;
	double phase = 0.0;

	int phase_i;
	int i, j;

	std::vector<int> LUT(LUT_SIZE, 0);

	for (i = 0; i < LUT_SIZE; i++)
		LUT[i] = (int) roundf(0x7FFF * sinf(2.0 * M_PI * i / LUT_SIZE));

	for (j = 0; j < SINE_SIZE; j++) {
		phase_i = (int) phase;
		tone_single_channel[j] = double(LUT[phase_i]);
		phase += delta_phi;
		if (phase >= LUT_SIZE) phase -= LUT_SIZE;
	}

	for (j = 0; j < SIZE; j += 2) {
		tone_dual_channel[j] = tone_single_channel[j / 2];
		tone_dual_channel[j + 1] = tone_dual_channel[j];
	}

    return std::make_tuple(tone_single_channel, tone_dual_channel);
}

void validate_pitch(std::string algo, double frequency, int sample_rate)
{
    auto x = generate_sine(frequency, sample_rate);
    auto sine = std::get<0>(x);
	double pitch = get_pitch(algo, sine, sample_rate);
	printf("Sinewave freq: %f\tpitch: %f\n", frequency, pitch);
}

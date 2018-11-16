#include "util.h"
#include <cmath>
#include <float.h>
#include <iterator>
#include <map>
#include <vector>

static std::map<double, std::string> NOTEMAP = {{16.35, "C0"}, {17.32, "C#0"},
    {18.35, "D0"}, {19.45, "Eb0"}, {20.6, "E0"}, {21.83, "F0"}, {23.12, "F#0"},
    {24.5, "G0"}, {25.96, "G#0"}, {27.5, "A0"}, {29.14, "Bb0"}, {30.87, "B0"},
    {32.7, "C1"}, {34.65, "C#1"}, {36.71, "D1"}, {38.89, "Eb1"}, {41.2, "E1"},
    {43.65, "F1"}, {46.25, "F#1"}, {49, "G1"}, {51.91, "G#1"}, {55, "A1"},
    {58.27, "Bb1"}, {61.74, "B1"}, {65.41, "C2"}, {69.3, "C#2"}, {73.42, "D2"},
    {77.78, "Eb2"}, {82.41, "E2"}, {87.31, "F2"}, {92.5, "F#2"}, {98, "G2"},
    {103.8, "G#2"}, {110, "A2"}, {116.5, "Bb2"}, {123.5, "B2"}, {130.8, "C3"},
    {138.6, "C#3"}, {146.8, "D3"}, {155.6, "Eb3"}, {164.8, "E3"}, {174.6, "F3"},
    {185, "F#3"}, {196, "G3"}, {207.7, "G#3"}, {220, "A3"}, {233.1, "Bb3"},
    {246.9, "B3"}, {261.6, "C4"}, {277.2, "C#4"}, {293.7, "D4"}, {311.1, "Eb4"},
    {329.6, "E4"}, {349.2, "F4"}, {370, "F#4"}, {392, "G4"}, {415.3, "G#4"},
    {440, "A4"}, {466.2, "Bb4"}, {493.9, "B4"}, {523.3, "C5"}, {554.4, "C#5"},
    {587.3, "D5"}, {622.3, "Eb5"}, {659.3, "E5"}, {698.5, "F5"}, {740, "F#5"},
    {784, "G5"}, {830.6, "G#5"}, {880, "A5"}, {932.3, "Bb5"}, {987.8, "B5"},
    {1047, "C6"}, {1109, "C#6"}, {1175, "D6"}, {1245, "Eb6"}, {1319, "E6"},
    {1397, "F6"}, {1480, "F#6"}, {1568, "G6"}, {1661, "G#6"}, {1760, "A6"},
    {1865, "Bb6"}, {1976, "B6"}, {2093, "C7"}, {2217, "C#7"}, {2349, "D7"},
    {2489, "Eb7"}, {2637, "E7"}, {2794, "F7"}, {2960, "F#7"}, {3136, "G7"},
    {3322, "G#7"}, {3520, "A7"}, {3729, "Bb7"}, {3951, "B7"}, {4186, "C8"},
    {4435, "C#8"}, {4699, "D8"}, {4978, "Eb8"}, {5274, "E8"}, {5588, "F8"},
    {5920, "F#8"}, {6272, "G8"}, {6645, "G#8"}, {7040, "A8"}, {7459, "Bb8"},
    {7902, "B8"}};

std::tuple<std::string, double>
util::closest_note(double pitch)
{
	std::map<double, std::string>::iterator it_lo, it_hi;

	it_hi = NOTEMAP.lower_bound(pitch);
	if (it_hi == NOTEMAP.begin() || it_hi == NOTEMAP.end()) {
		return std::make_tuple("n/a", pitch);
	}

	it_lo = std::prev(it_hi, 1);

	if ((pitch - it_lo->first) <= (it_hi->first - pitch)) {
		return std::make_tuple(it_lo->second, it_lo->first);
	}
	return std::make_tuple(it_hi->second, it_hi->first);
}

std::vector<double>
util::generate_sinewave(size_t size, double frequency, int sample_rate)
{
	size_t lut_size = size / 4;

	std::vector<int> lut{};
	double *_tone_single_channel = (double *)malloc(sizeof(double) * size / 2);

	double doublef = (double)frequency;
	double delta_phi = doublef * lut_size * 1.0 / sample_rate;
	double phase = 0.0;

	for (int i = 0; i < signed(lut_size); ++i) {
		lut.push_back((int)roundf(0x7FFF * sinf(2.0 * M_PI * i / lut_size)));
	}

	double min = DBL_MAX;
	double max = -DBL_MAX;
	for (int i = 0; i < signed(size / 2); ++i) {
		int val = double(lut[(int)phase]);
		if (val > max) {
			max = val;
		}
		if (val < min) {
			min = val;
		}
		_tone_single_channel[i] = val;
		phase += delta_phi;
		if (phase >= lut_size)
			phase -= lut_size;
	}

	std::vector<double> tone_single_channel(
	    _tone_single_channel, _tone_single_channel + size / 2);

	return tone_single_channel;
}

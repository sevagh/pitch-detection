#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

class PYinSinewaveTest : public testing::TestWithParam<double>
{
};

TEST_P(PYinSinewaveTest, GetFreqManualAlloc)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	pitch_alloc::Yin<double> pya(data.size());
	auto pitches = pitch_alloc::pyin(data, 48000, &pya);

	double best_pitch = -1.0;
	double highest_prob = -1.0;
	for (auto pitch : pitches) {
		std::cout << "PYin pitch: " << pitch.first
		          << "\tprobability: " << pitch.second << std::endl;

		// pitch with highest probability "wins"
		if (pitch.second > highest_prob) {
			highest_prob = pitch.second;
			best_pitch = pitch.first;
		}
	}
	EXPECT_NEAR(freq, best_pitch, 0.01 * freq);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTest,
    ::testing::Values(77.0, 100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

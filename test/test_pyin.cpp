#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

void
pyin_helper(
    std::vector<std::pair<double, double>> pitches, double expected_freq)
{
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
	EXPECT_NEAR(expected_freq, best_pitch, 0.01 * expected_freq);
}

class PYinSinewaveTest : public testing::TestWithParam<double>
{
};

TEST_P(PYinSinewaveTest, GetFreqManualAlloc)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	pitch_alloc::Yin<double> pya(data.size());
	auto pitches = pitch_alloc::pyin(data, 48000, &pya);
	pyin_helper(pitches, freq);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTest,
    ::testing::Values(77.0, 100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

TEST(PYinInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/sample/A4_44100_violin.txt");
	auto pitches = pitch::pyin<double>(data, 44100);
	double expected = 440.0;
	pyin_helper(pitches, expected);
}

TEST(PYinInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/sample/B4_44100_piano.txt");
	auto pitches = pitch::pyin<double>(data, 44100);
	double expected = 493.9;
	pyin_helper(pitches, expected);
}

TEST(PYinInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/sample/D4_44100_piano.txt");
	auto pitches = pitch::pyin<double>(data, 44100);
	double expected = 293.7;
	pyin_helper(pitches, expected);
}

TEST(PYinInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/sample/E2_44100_acousticguitar.txt");
	auto pitches = pitch::pyin<double>(data, 44100);
	double expected = 82.41;
	pyin_helper(pitches, expected);
}

TEST(PYinInstrumentTest, Classical_FSharp4_48000)
{
	auto data =
	    test_util::vec_from_file("./misc/sample/F-4_48000_classicalguitar.txt");
	auto pitches = pitch::pyin<double>(data, 48000);
	double expected = 370.0;
	pyin_helper(pitches, expected);
}

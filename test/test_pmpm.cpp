#include "pitch_detection/pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

void
pmpm_helper(
    std::vector<std::pair<double, double>> pitches, double expected_freq)
{
	double best_pitch = -1.0;
	double highest_prob = -1.0;
	for (auto pitch : pitches) {
		std::cout << "PMpm pitch: " << pitch.first
		          << "\tprobability: " << pitch.second << std::endl;

		// pitch with highest probability "wins"
		if (pitch.second > highest_prob) {
			highest_prob = pitch.second;
			best_pitch = pitch.first;
		}
	}
	EXPECT_NEAR(expected_freq, best_pitch, 0.01 * expected_freq);
}

class PMpmSinewaveTest : public testing::TestWithParam<double>
{
};

TEST_P(PMpmSinewaveTest, GetFreqManualAlloc)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	pitch_alloc::Mpm<double> ma(data.size());
	auto pitches = ma.probabilistic_pitches(data, 48000);

	pmpm_helper(pitches, freq);
}

INSTANTIATE_TEST_CASE_P(PMpmSinewave, PMpmSinewaveTest,
    ::testing::Values(100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

TEST(PMpmInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/A4_44100_violin.txt");
	auto pitches = pitch::pmpm<double>(data, 44100);
	double expected = 440.0;
	pmpm_helper(pitches, expected);
}

TEST(PMpmInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/B4_44100_piano.txt");
	auto pitches = pitch::pmpm<double>(data, 44100);
	double expected = 493.9;
	pmpm_helper(pitches, expected);
}

TEST(PMpmInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/D4_44100_piano.txt");
	auto pitches = pitch::pmpm<double>(data, 44100);
	double expected = 293.7;
	pmpm_helper(pitches, expected);
}

TEST(PMpmInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/samples/E2_44100_acousticguitar.txt");
	auto pitches = pitch::pmpm<double>(data, 44100);
	double expected = 82.41;
	pmpm_helper(pitches, expected);
}

TEST(PMpmInstrumentTest, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	auto pitches = pitch::pmpm<double>(data, 48000);
	double expected = 370.0;
	pmpm_helper(pitches, expected);
}

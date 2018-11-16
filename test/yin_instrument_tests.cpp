#include "test.h"
#include <gtest/gtest.h>
#include <pitch_detection.h>

TEST(YinInstrumentTest, Violin_A4_44100)
{
	auto data = vec_from_file("./sample/A4_44100_violin.txt");
	double pitch = get_pitch_yin(data, 44100);
	double expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Piano_B4_44100)
{
	auto data = vec_from_file("./sample/B4_44100_piano.txt");
	double pitch = get_pitch_yin(data, 44100);
	double expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Piano_D4_44100)
{
	auto data = vec_from_file("./sample/D4_44100_piano.txt");
	double pitch = get_pitch_yin(data, 44100);
	double expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Acoustic_E2_44100)
{
	auto data = vec_from_file("./sample/E2_44100_acousticguitar.txt");
	double pitch = get_pitch_yin(data, 44100);
	double expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Classical_FSharp4_48000)
{
	auto data = vec_from_file("./sample/F-4_48000_classicalguitar.txt");
	double pitch = get_pitch_yin(data, 48000);
	double expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

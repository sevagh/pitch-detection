#include "test.h"
#include <gtest/gtest.h>
#include <pitch_detection.h>

TEST(MpmInstrumentTest, Violin_A4_44100)
{
	auto data = vec_from_file("./misc/sample/A4_44100_violin.txt");
	double pitch = pitch::mpm(data, 44100);
	double expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Piano_B4_44100)
{
	auto data = vec_from_file("./misc/sample/B4_44100_piano.txt");
	double pitch = pitch::mpm(data, 44100);
	double expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Piano_D4_44100)
{
	auto data = vec_from_file("./misc/sample/D4_44100_piano.txt");
	double pitch = pitch::mpm(data, 44100);
	double expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Acoustic_E2_44100)
{
	auto data = vec_from_file("./misc/sample/E2_44100_acousticguitar.txt");
	double pitch = pitch::mpm(data, 44100);
	double expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Classical_FSharp4_48000)
{
	auto data = vec_from_file("./misc/sample/F-4_48000_classicalguitar.txt");
	double pitch = pitch::mpm(data, 48000);
	double expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

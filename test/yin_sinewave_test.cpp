#include "test.h"
#include <gtest/gtest.h>
#include <pitch_detection.h>
#include <util.h>

TEST(YinSinewaveTest, Sinewave_77Hz)
{
	double freq = 77.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(YinSinewaveTest, Sinewave_100Hz)
{
	double freq = 100.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(YinSinewaveTest, Sinewave_233Hz)
{
	double freq = 233.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(YinSinewaveTest, Sinewave_298Hz)
{
	double freq = 298.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(YinSinewaveTest, Sinewave_1583Hz)
{
	double freq = 1583.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(YinSinewaveTest, Sinewave_3398Hz)
{
	double freq = 3398.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(YinSinewaveTest, Sinewave_4200Hz)
{
	double freq = 4200.0;
	auto data = util::sinewave(4096, freq, 48000);
	double pitch = get_pitch_yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

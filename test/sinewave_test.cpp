#include "test.h"
#include <gtest/gtest.h>
#include <pitch_detection.h>
#include <util.h>

class MpmSinewaveTest : public testing::TestWithParam<double>
{
};
class YinSinewaveTest : public testing::TestWithParam<double>
{
};

TEST_P(MpmSinewaveTest, GetFreq)
{
	double freq = GetParam();
	auto data = util::sinewave(8092, freq, 48000);
	double pitch = pitch::mpm(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreq)
{
	double freq = GetParam();
	auto data = util::sinewave(8092, freq, 48000);
	double pitch = pitch::yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

INSTANTIATE_TEST_CASE_P(MpmSinewave, MpmSinewaveTest,
    ::testing::Values(77.0, 100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

INSTANTIATE_TEST_CASE_P(YinSinewave, YinSinewaveTest,
    ::testing::Values(77.0, 100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

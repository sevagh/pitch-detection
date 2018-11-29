#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

class MpmSinewaveTest : public testing::TestWithParam<double>
{
};

class YinSinewaveTest : public testing::TestWithParam<double>
{
};

TEST_P(MpmSinewaveTest, GetFreq)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	double pitch = pitch::mpm(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreq)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	double pitch = pitch::yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTest, GetFreqManualAlloc)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	pitch_alloc::Mpm ma(data.size());
	double pitch = pitch_alloc::mpm(data, 48000, &ma);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreqManualAlloc)
{
	double freq = GetParam();
	auto data = test_util::sinewave(8092, freq, 48000);
	pitch_alloc::Yin ya(data.size());
	double pitch = pitch_alloc::yin(data, 48000, &ya);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(MpmSinewaveTestManualAlloc, OneAllocMultipleFreq)
{
	auto data1 = test_util::sinewave(8092, 150.0, 48000);
	auto data2 = test_util::sinewave(8092, 250.0, 48000);
	auto data3 = test_util::sinewave(8092, 350.0, 48000);

	pitch_alloc::Mpm ma(data1.size());

	double pitch1 = pitch_alloc::mpm(data1, 48000, &ma);
	double pitch2 = pitch_alloc::mpm(data2, 48000, &ma);
	double pitch3 = pitch_alloc::mpm(data3, 48000, &ma);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAlloc, OneAllocMultipleFreq)
{
	auto data1 = test_util::sinewave(8092, 150.0, 48000);
	auto data2 = test_util::sinewave(8092, 250.0, 48000);
	auto data3 = test_util::sinewave(8092, 350.0, 48000);

	pitch_alloc::Yin ya(data1.size());

	double pitch1 = pitch_alloc::yin(data1, 48000, &ya);
	double pitch2 = pitch_alloc::yin(data2, 48000, &ya);
	double pitch3 = pitch_alloc::yin(data3, 48000, &ya);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

// no 77.0hz for mpm because it can't
INSTANTIATE_TEST_CASE_P(MpmSinewave, MpmSinewaveTest,
    ::testing::Values(100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

INSTANTIATE_TEST_CASE_P(YinSinewave, YinSinewaveTest,
    ::testing::Values(77.0, 100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

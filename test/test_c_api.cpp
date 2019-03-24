#include "pitch_detection/cpitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

TEST(CAPITest, MpmNoStruct)
{
	double freq = 1337.0;
	auto data = test_util::sinewave(8092, freq, 48000);
	double pitch = pitch_mpm_d(data.data(), data.size(), 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(CAPITest, YinNoStruct)
{
	double freq = 1337.0;
	auto data = test_util::sinewave(8092, freq, 48000);
	double pitch = pitch_yin_d(data.data(), data.size(), 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(CAPITest, MpmStruct)
{
	double freq = 1337.0;
	auto data = test_util::sinewave(8092, freq, 48000);
	struct Mpm_d_t *m = NewMpmD(data.size());
	double pitch = pitch_alloc_mpm_d(m, data.data(), 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
	DeleteMpmD(m);
}

TEST(CAPITest, YinStruct)
{
	double freq = 1337.0;
	auto data = test_util::sinewave(8092, freq, 48000);
	struct Yin_d_t *y = NewYinD(data.size());
	double pitch = pitch_alloc_yin_d(y, data.data(), 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
	DeleteYinD(y);
}

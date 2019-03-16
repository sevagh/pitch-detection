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
	pitch_alloc::PYin<double> pya(data.size());
	double pitch = pitch_alloc::pyin(data, 48000, &pya);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTest,
    ::testing::Values(77.0, 100.0, 233.0, 298.0, 1583.0, 3398.0, 4200.0));

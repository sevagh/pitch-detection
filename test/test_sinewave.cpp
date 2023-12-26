#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

class MpmSinewaveTest : public testing::TestWithParam<int>
{
};

class YinSinewaveTest : public testing::TestWithParam<int>
{
};

class PMpmSinewaveTest : public testing::TestWithParam<int>
{
};

class PYinSinewaveTest : public testing::TestWithParam<int>
{
};

TEST_P(PYinSinewaveTest, GetFreqManualAlloc)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file("./misc/samples/sine_"+std::to_string(freq)+"_0.txt");

	// yin must be initialized with 8092
	// 8192 or anything else = incorrect!
	pitch_alloc::Yin pya(data.size());

	float pitch = pya.probabilistic_pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTest,
    ::testing::Values(77, 100, 233, 298, 1583, 3398, 4200));

TEST_P(PMpmSinewaveTest, GetFreqManualAlloc)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file("./misc/samples/sine_"+std::to_string(freq)+"_0.txt");
	pitch_alloc::Mpm pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

INSTANTIATE_TEST_CASE_P(PMpmSinewave, PMpmSinewaveTest,
    ::testing::Values(100, 233, 298, 1583, 3398, 4200));

TEST_P(MpmSinewaveTest, GetFreq)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file("./misc/samples/sine_"+std::to_string(freq)+"_0.txt");
	float pitch = pitch::mpm(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreq)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file("./misc/samples/sine_"+std::to_string(freq)+"_0.txt");
	float pitch = pitch::yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTest, GetFreqManualAlloc)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file("./misc/samples/sine_"+std::to_string(freq)+"_0.txt");
	pitch_alloc::Mpm ma(data.size());
	float pitch = ma.pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreqManualAlloc)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file("./misc/samples/sine_"+std::to_string(freq)+"_0.txt");
	pitch_alloc::Yin ya(data.size());
	float pitch = ya.pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST(MpmSinewaveTestManualAlloc, OneAllocMultipleFreq)
{
	auto data1 = test_util::vec_from_file("./misc/samples/sine_150_0.txt");
	auto data2 = test_util::vec_from_file("./misc/samples/sine_250_0.txt");
	auto data3 = test_util::vec_from_file("./misc/samples/sine_350_0.txt");

        std::cout << "data1 size: " << data1.size() << std::endl;

	pitch_alloc::Mpm ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAlloc, OneAllocMultipleFreq)
{
	auto data1 = test_util::vec_from_file("./misc/samples/sine_150_0.txt");
	auto data2 = test_util::vec_from_file("./misc/samples/sine_250_0.txt");
	auto data3 = test_util::vec_from_file("./misc/samples/sine_350_0.txt");

	pitch_alloc::Yin ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

// no 77.0hz for mpm because it can't
INSTANTIATE_TEST_CASE_P(MpmSinewave, MpmSinewaveTest,
    ::testing::Values(100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(YinSinewave, YinSinewaveTest,
    ::testing::Values(77, 100, 233, 298, 1583, 3398, 4200));

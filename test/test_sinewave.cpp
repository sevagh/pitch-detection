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

TEST_P(PYinSinewaveTest, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Yin pya(data.size());
	float pitch = pya.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(PMpmSinewaveTest, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Mpm pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}


TEST_P(MpmSinewaveTest, GetFreqFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	float pitch = pitch::mpm(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreqFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	float pitch = pitch::yin(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(MpmSinewaveTest, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Mpm ma(data.size());
	float pitch = ma.pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTest, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Yin ya(data.size());
	float pitch = ya.pitch(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST(MpmSinewaveTestManualAlloc, OneAllocMultipleFreqFromFile)
{
	auto data1 = test_util::vec_from_file("./misc/samples/sine_150_0.txt");
	auto data2 = test_util::vec_from_file("./misc/samples/sine_250_0.txt");
	auto data3 = test_util::vec_from_file("./misc/samples/sine_350_0.txt");

	pitch_alloc::Mpm ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAlloc, OneAllocMultipleFreqFromFile)
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

TEST(AllAlgorithmsSineWave, PowerOfTwo)
/*
testcases proposed by @jeychenne
in this issue: https://github.com/sevagh/pitch-detection/issues/72
*/
{
	std::cerr << "Expected pitch: 250 Hz" << std::endl;

	auto d = test_util::sinewave(2048, 250, 16000);
	auto p = pitch::yin(d, 16000);
	std::cerr << "pitch with 2048 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(256, 250, 16000);
	p = pitch::yin(d, 16000);
	std::cerr << "pitch with 256 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(4096, 250, 16000);
	p = pitch::yin(d, 16000);
	std::cerr << "pitch with 4096 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(2048, 250, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 2048 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(4096, 250, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 4096 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(8092, 250, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 8092 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(8092, 250, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 8092 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(8192, 250, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 8192 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(8192, 250, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 8192 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(4046, 250, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 4046 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(4046, 250, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 4046 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(15000, 250, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 15000 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;

	d = test_util::sinewave(16384, 250, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 16384 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
}

TEST_P(PYinSinewaveTest, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave(8192, freq, 48000);
	pitch_alloc::Yin pya(data.size());
	float pitch = pya.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(PMpmSinewaveTest, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave(8000, freq, 48000);
	pitch_alloc::Mpm pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(MpmSinewaveTest, GetFreqGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave(8192, freq, 48000);
	float pitch = pitch::mpm(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(YinSinewaveTest, GetFreqGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave(67893, freq, 48000);
	float pitch = pitch::yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTest, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave(16384, freq, 48000);
	pitch_alloc::Mpm ma(data.size());
	float pitch = ma.pitch(data, 48000);
	// allow worse precision for frequencies < 100 Hz

	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(YinSinewaveTest, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave(16384, freq, 48000);
	pitch_alloc::Yin ya(data.size());
	float pitch = ya.pitch(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST(MpmSinewaveTestManualAlloc, OneAllocMultipleFreqGenerated)
{
	auto data1 = test_util::sinewave(8192, 150, 48000);
	auto data2 = test_util::sinewave(8192, 250, 48000);
	auto data3 = test_util::sinewave(8192, 350, 48000);

	pitch_alloc::Mpm ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAlloc, OneAllocMultipleFreqGeneratedPowerOfTwo)
{
	auto data1 = test_util::sinewave(8192, 150, 48000);
	auto data2 = test_util::sinewave(8192, 250, 48000);
	auto data3 = test_util::sinewave(8192, 350, 48000);

	pitch_alloc::Yin ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.025 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.025 * 350.0);
}

TEST(YinSinewaveTestManualAlloc, OneAllocMultipleFreqGeneratedNonPowerOfTwo)
{
	auto data1 = test_util::sinewave(8092, 150, 48000);
	auto data2 = test_util::sinewave(8092, 250, 48000);
	auto data3 = test_util::sinewave(8092, 350, 48000);

	pitch_alloc::Yin ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTest,
    ::testing::Values(77, 83, 100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(YinSinewave, YinSinewaveTest,
    ::testing::Values(77, 83, 100, 233, 298, 1583, 3398, 4200));

// no 77.0hz for mpm because it can't go that low
INSTANTIATE_TEST_CASE_P(PMpmSinewave, PMpmSinewaveTest,
    ::testing::Values(83, 100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(MpmSinewave, MpmSinewaveTest,
    ::testing::Values(83, 100, 233, 298, 1583, 3398, 4200));

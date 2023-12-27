#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

class MpmSinewaveTestFloat : public testing::TestWithParam<int>
{
};

class YinSinewaveTestFloat : public testing::TestWithParam<int>
{
};

class PMpmSinewaveTestFloat : public testing::TestWithParam<int>
{
};

class PYinSinewaveTestFloat : public testing::TestWithParam<int>
{
};

TEST_P(PYinSinewaveTestFloat, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Yin<float> pya(data.size());
	float pitch = pya.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(PMpmSinewaveTestFloat, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Mpm<float> pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTestFloat, GetFreqFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	float pitch = pitch::mpm(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTestFloat, GetFreqFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	float pitch = pitch::yin(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(MpmSinewaveTestFloat, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Mpm<float> ma(data.size());
	float pitch = ma.pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTestFloat, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Yin<float> ya(data.size());
	float pitch = ya.pitch(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST(MpmSinewaveTestManualAllocFloat, OneAllocMultipleFreqFromFile)
{
	auto data1 =
	    test_util::vec_from_file<float>("./misc/samples/sine_150_0.txt");
	auto data2 =
	    test_util::vec_from_file<float>("./misc/samples/sine_250_0.txt");
	auto data3 =
	    test_util::vec_from_file<float>("./misc/samples/sine_350_0.txt");

	pitch_alloc::Mpm<float> ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAllocFloat, OneAllocMultipleFreqFromFile)
{
	auto data1 =
	    test_util::vec_from_file<float>("./misc/samples/sine_150_0.txt");
	auto data2 =
	    test_util::vec_from_file<float>("./misc/samples/sine_250_0.txt");
	auto data3 =
	    test_util::vec_from_file<float>("./misc/samples/sine_350_0.txt");

	pitch_alloc::Yin<float> ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(AllAlgorithmsSineWaveFloat, PowerOfTwo)
/*
testcases proposed by @jeychenne
in this issue: https://github.com/sevagh/pitch-detection/issues/72
*/
{
	std::cerr << "Expected pitch: 250 Hz" << std::endl;

	float freq = 250.0f;
	float tol = 0.01;

	auto d = test_util::sinewave<float>(2048, freq, 16000);
	auto p = pitch::yin(d, 16000);
	std::cerr << "pitch with 2048 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(256, freq, 16000);
	p = pitch::yin(d, 16000);
	std::cerr << "pitch with 256 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;
	tol = 0.05;
	EXPECT_NEAR(freq, p, tol * freq);
	tol = 0.01;

	d = test_util::sinewave<float>(4096, freq, 16000);
	p = pitch::yin(d, 16000);
	std::cerr << "pitch with 4096 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(2048, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 2048 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	tol = 0.05;
	EXPECT_NEAR(freq, p, tol * freq);
	tol = 0.01;

	d = test_util::sinewave<float>(4096, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 4096 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(8092, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 8092 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(8092, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 8092 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(8192, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 8192 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(8192, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 8192 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(4046, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 4046 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(4046, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 4046 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(15000, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 15000 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<float>(16384, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 16384 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);
}

TEST_P(PYinSinewaveTestFloat, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<float>(8192, freq, 48000);
	pitch_alloc::Yin<float> pya(data.size());
	float pitch = pya.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(PMpmSinewaveTestFloat, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<float>(8000, freq, 48000);
	pitch_alloc::Mpm<float> pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(MpmSinewaveTestFloat, GetFreqGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<float>(8192, freq, 48000);
	float pitch = pitch::mpm(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(YinSinewaveTestFloat, GetFreqGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<float>(67893, freq, 48000);
	float pitch = pitch::yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTestFloat, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<float>(16384, freq, 48000);
	pitch_alloc::Mpm<float> ma(data.size());
	float pitch = ma.pitch(data, 48000);
	// allow worse precision for frequencies < 100 Hz

	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(YinSinewaveTestFloat, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<float>(16384, freq, 48000);
	pitch_alloc::Yin<float> ya(data.size());
	float pitch = ya.pitch(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST(MpmSinewaveTestManualAllocFloat, OneAllocMultipleFreqGenerated)
{
	auto data1 = test_util::sinewave<float>(8192, 150, 48000);
	auto data2 = test_util::sinewave<float>(8192, 250, 48000);
	auto data3 = test_util::sinewave<float>(8192, 350, 48000);

	pitch_alloc::Mpm<float> ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAllocFloat, OneAllocMultipleFreqGeneratedPowerOfTwo)
{
	auto data1 = test_util::sinewave<float>(8192, 150, 48000);
	auto data2 = test_util::sinewave<float>(8192, 250, 48000);
	auto data3 = test_util::sinewave<float>(8192, 350, 48000);

	pitch_alloc::Yin<float> ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.025 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.025 * 350.0);
}

TEST(
    YinSinewaveTestManualAllocFloat, OneAllocMultipleFreqGeneratedNonPowerOfTwo)
{
	auto data1 = test_util::sinewave<float>(8092, 150, 48000);
	auto data2 = test_util::sinewave<float>(8092, 250, 48000);
	auto data3 = test_util::sinewave<float>(8092, 350, 48000);

	pitch_alloc::Yin<float> ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTestFloat,
    ::testing::Values(77, 83, 100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(YinSinewave, YinSinewaveTestFloat,
    ::testing::Values(77, 83, 100, 233, 298, 1583, 3398, 4200));

// no 77.0hz for mpm because it can't go that low
INSTANTIATE_TEST_CASE_P(PMpmSinewave, PMpmSinewaveTestFloat,
    ::testing::Values(83, 100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(MpmSinewave, MpmSinewaveTestFloat,
    ::testing::Values(83, 100, 233, 298, 1583, 3398, 4200));

class MpmSinewaveTestDouble : public testing::TestWithParam<int>
{
};

class YinSinewaveTestDouble : public testing::TestWithParam<int>
{
};

class PMpmSinewaveTestDouble : public testing::TestWithParam<int>
{
};

class PYinSinewaveTestDouble : public testing::TestWithParam<int>
{
};

TEST_P(PYinSinewaveTestDouble, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Yin<double> pya(data.size());
	float pitch = pya.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(PMpmSinewaveTestDouble, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Mpm<double> pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTestDouble, GetFreqFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	float pitch = pitch::mpm(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTestDouble, GetFreqFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	float pitch = pitch::yin(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(MpmSinewaveTestDouble, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Mpm<double> ma(data.size());
	float pitch = ma.pitch(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(YinSinewaveTestDouble, GetFreqManualAllocFromFile)
{
	int freq = GetParam();
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/sine_" + std::to_string(freq) + "_0.txt");
	pitch_alloc::Yin<double> ya(data.size());
	float pitch = ya.pitch(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST(MpmSinewaveTestManualAllocDouble, OneAllocMultipleFreqFromFile)
{
	auto data1 =
	    test_util::vec_from_file<double>("./misc/samples/sine_150_0.txt");
	auto data2 =
	    test_util::vec_from_file<double>("./misc/samples/sine_250_0.txt");
	auto data3 =
	    test_util::vec_from_file<double>("./misc/samples/sine_350_0.txt");

	pitch_alloc::Mpm<double> ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAllocDouble, OneAllocMultipleFreqFromFile)
{
	auto data1 =
	    test_util::vec_from_file<double>("./misc/samples/sine_150_0.txt");
	auto data2 =
	    test_util::vec_from_file<double>("./misc/samples/sine_250_0.txt");
	auto data3 =
	    test_util::vec_from_file<double>("./misc/samples/sine_350_0.txt");

	pitch_alloc::Yin<double> ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.01 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(AllAlgorithmsSineWaveDouble, PowerOfTwo)
/*
testcases proposed by @jeychenne
in this issue: https://github.com/sevagh/pitch-detection/issues/72
*/
{
	std::cerr << "Expected pitch: 250 Hz" << std::endl;

	float freq = 250.0f;
	float tol = 0.01;

	auto d = test_util::sinewave<double>(2048, freq, 16000);
	auto p = pitch::yin(d, 16000);
	std::cerr << "pitch with 2048 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(256, freq, 16000);
	p = pitch::yin(d, 16000);
	std::cerr << "pitch with 256 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;
	tol = 0.05;
	EXPECT_NEAR(freq, p, tol * freq);
	tol = 0.01;

	d = test_util::sinewave<double>(4096, freq, 16000);
	p = pitch::yin(d, 16000);
	std::cerr << "pitch with 4096 samples @ 16000 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(2048, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 2048 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	tol = 0.05;
	EXPECT_NEAR(freq, p, tol * freq);
	tol = 0.01;

	d = test_util::sinewave<double>(4096, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 4096 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(8092, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 8092 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(8092, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 8092 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(8192, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 8192 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(8192, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 8192 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(4046, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 4046 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(4046, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 4046 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(15000, freq, 22050);
	p = pitch::yin(d, 22050);
	std::cerr << "pitch with 15000 samples @ 22050 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);

	d = test_util::sinewave<double>(16384, freq, 48000);
	p = pitch::yin(d, 48000);
	std::cerr << "pitch with 16384 samples @ 4800 Hz: " << p << " Hz"
	          << std::endl;
	EXPECT_NEAR(freq, p, tol * freq);
}

TEST_P(PYinSinewaveTestDouble, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<double>(8192, freq, 48000);
	pitch_alloc::Yin<double> pya(data.size());
	float pitch = pya.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(PMpmSinewaveTestDouble, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<double>(8000, freq, 48000);
	pitch_alloc::Mpm<double> pma(data.size());
	float pitch = pma.probabilistic_pitch(data, 48000);
	double tolerance = 0.01;
	if (freq <= 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(MpmSinewaveTestDouble, GetFreqGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<double>(8192, freq, 48000);
	float pitch = pitch::mpm(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(YinSinewaveTestDouble, GetFreqGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<double>(67893, freq, 48000);
	float pitch = pitch::yin(data, 48000);
	EXPECT_NEAR(freq, pitch, 0.01 * freq);
}

TEST_P(MpmSinewaveTestDouble, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<double>(16384, freq, 48000);
	pitch_alloc::Mpm<double> ma(data.size());
	float pitch = ma.pitch(data, 48000);
	// allow worse precision for frequencies < 100 Hz

	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST_P(YinSinewaveTestDouble, GetFreqManualAllocGenerated)
{
	int freq = GetParam();
	auto data = test_util::sinewave<double>(16384, freq, 48000);
	pitch_alloc::Yin<double> ya(data.size());
	float pitch = ya.pitch(data, 48000);
	double tolerance = 0.01;
	if (freq < 100)
		tolerance = 0.05;
	EXPECT_NEAR(freq, pitch, tolerance * freq);
}

TEST(MpmSinewaveTestManualAllocDouble, OneAllocMultipleFreqGenerated)
{
	auto data1 = test_util::sinewave<double>(8192, 150, 48000);
	auto data2 = test_util::sinewave<double>(8192, 250, 48000);
	auto data3 = test_util::sinewave<double>(8192, 350, 48000);

	pitch_alloc::Mpm<double> ma(data1.size());

	float pitch1 = ma.pitch(data1, 48000);
	float pitch2 = ma.pitch(data2, 48000);
	float pitch3 = ma.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

TEST(YinSinewaveTestManualAllocDouble, OneAllocMultipleFreqGeneratedPowerOfTwo)
{
	auto data1 = test_util::sinewave<double>(8192, 150, 48000);
	auto data2 = test_util::sinewave<double>(8192, 250, 48000);
	auto data3 = test_util::sinewave<double>(8192, 350, 48000);

	pitch_alloc::Yin<double> ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.025 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.025 * 350.0);
}

TEST(YinSinewaveTestManualAllocDouble,
    OneAllocMultipleFreqGeneratedNonPowerOfTwo)
{
	auto data1 = test_util::sinewave<double>(8092, 150, 48000);
	auto data2 = test_util::sinewave<double>(8092, 250, 48000);
	auto data3 = test_util::sinewave<double>(8092, 350, 48000);

	pitch_alloc::Yin<double> ya(data1.size());

	float pitch1 = ya.pitch(data1, 48000);
	float pitch2 = ya.pitch(data2, 48000);
	float pitch3 = ya.pitch(data3, 48000);

	EXPECT_NEAR(150.0, pitch1, 0.025 * 150.0);
	EXPECT_NEAR(250.0, pitch2, 0.01 * 250.0);
	EXPECT_NEAR(350.0, pitch3, 0.01 * 350.0);
}

INSTANTIATE_TEST_CASE_P(PYinSinewave, PYinSinewaveTestDouble,
    ::testing::Values(77, 83, 100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(YinSinewave, YinSinewaveTestDouble,
    ::testing::Values(77, 83, 100, 233, 298, 1583, 3398, 4200));

// no 77.0hz for mpm because it can't go that low
INSTANTIATE_TEST_CASE_P(PMpmSinewave, PMpmSinewaveTestDouble,
    ::testing::Values(83, 100, 233, 298, 1583, 3398, 4200));

INSTANTIATE_TEST_CASE_P(MpmSinewave, MpmSinewaveTestDouble,
    ::testing::Values(83, 100, 233, 298, 1583, 3398, 4200));

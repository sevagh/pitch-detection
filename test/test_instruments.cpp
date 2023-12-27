#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

TEST(YinInstrumentTestFloat, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::yin<float>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestFloat, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::yin<float>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestFloat, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::yin<float>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestFloat, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::yin<float>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestFloat, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::yin<float>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestFloat, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::mpm<float>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestFloat, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::mpm<float>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestFloat, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::mpm<float>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestFloat, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::mpm<float>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestFloat, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::mpm<float>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestFloat, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::pyin<float>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestFloat, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::pyin<float>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestFloat, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::pyin<float>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestFloat, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::pyin<float>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestFloat, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::pyin<float>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestFloat, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::pmpm<float>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestFloat, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::pmpm<float>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestFloat, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<float>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::pmpm<float>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestFloat, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::pmpm<float>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestFloat, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<float>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::pmpm<float>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestDouble, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::yin<double>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestDouble, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::yin<double>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestDouble, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::yin<double>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestDouble, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::yin<double>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTestDouble, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::yin<double>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestDouble, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::mpm<double>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestDouble, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::mpm<double>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestDouble, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::mpm<double>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestDouble, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::mpm<double>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTestDouble, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::mpm<double>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestDouble, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::pyin<double>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestDouble, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::pyin<double>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestDouble, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::pyin<double>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestDouble, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::pyin<double>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTestDouble, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::pyin<double>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestDouble, Violin_A4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::pmpm<double>(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestDouble, Piano_B4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::pmpm<double>(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestDouble, Piano_D4_44100)
{
	auto data =
	    test_util::vec_from_file<double>("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::pmpm<double>(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestDouble, Acoustic_E2_44100)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::pmpm<double>(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTestDouble, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file<double>(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::pmpm<double>(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

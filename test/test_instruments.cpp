#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

TEST(YinInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::yin(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::yin(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::yin(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::yin(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(YinInstrumentTest, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::yin(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::mpm(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::mpm(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::mpm(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::mpm(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(MpmInstrumentTest, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::mpm(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(DISABLED_SwipeInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/A4_44100_violin.txt");

	float pitch = pitch::swipe(data, 48000);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(DISABLED_SwipeInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/B4_44100_piano.txt");

	float pitch = pitch::swipe(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(DISABLED_SwipeInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/D4_44100_piano.txt");

	float pitch = pitch::swipe(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(DISABLED_SwipeInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/samples/E2_44100_acousticguitar.txt");

	float pitch = pitch::swipe(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(DISABLED_SwipeInstrumentTest, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file(
	    "./misc/samples/F-4_48000_classicalguitar.txt");

	float pitch = pitch::swipe(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::pyin(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::pyin(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::pyin(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::pyin(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PYinInstrumentTest, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::pyin(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/A4_44100_violin.txt");
	float pitch = pitch::pmpm(data, 44100);
	float expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTest, Piano_B4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/B4_44100_piano.txt");
	float pitch = pitch::pmpm(data, 44100);
	float expected = 493.9;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTest, Piano_D4_44100)
{
	auto data = test_util::vec_from_file("./misc/samples/D4_44100_piano.txt");
	float pitch = pitch::pmpm(data, 44100);
	float expected = 293.7;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTest, Acoustic_E2_44100)
{
	auto data =
	    test_util::vec_from_file("./misc/samples/E2_44100_acousticguitar.txt");
	float pitch = pitch::pmpm(data, 44100);
	float expected = 82.41;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

TEST(PMpmInstrumentTest, Classical_FSharp4_48000)
{
	auto data = test_util::vec_from_file(
	    "./misc/samples/F-4_48000_classicalguitar.txt");
	float pitch = pitch::pmpm(data, 48000);
	float expected = 370.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

#include "pitch_detection/pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>

TEST(PYinInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/sample/A4_44100_violin.txt");
	double pitch = pitch::pyin(data, 44100);
	double expected = 440.0;
	EXPECT_NEAR(expected, pitch, 0.01 * expected);
}

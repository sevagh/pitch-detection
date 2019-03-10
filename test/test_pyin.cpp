#include "pitch_detection.h"
#include "util.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(PYinInstrumentTest, Violin_A4_44100)
{
	auto data = test_util::vec_from_file("./misc/sample/A4_44100_violin.txt");
	double pitch_pyin = pitch::pyin(data, 44100);
	double pitch_mpm = pitch::mpm(data, 44100);
	double pitch_yin = pitch::yin(data, 44100);
	double expected = 440.0;

	std::cout << pitch_pyin << "\t" << pitch_yin << "\t" << pitch_mpm << std::endl;
	EXPECT_NEAR(expected, pitch_pyin, 0.01 * expected);
}

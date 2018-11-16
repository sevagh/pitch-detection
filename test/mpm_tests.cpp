#include "test.h"
#include <gtest/gtest.h>
#include <pitch_detection.h>

TEST(MpmTest, Violin_A4_44100)
{
	auto data = vec_from_file("../samples/A4_44100_violin.txt");
	double pitch = get_pitch_mpm(data, 44100);

	EXPECT_NEAR(440.0, pitch, 4.4);
}

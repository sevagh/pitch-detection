#include <gtest/gtest.h>
#include <pitch_detection.h>

TEST(MpmEdgeCase, EmptyData)
{
	EXPECT_THROW(
	    pitch::mpm(std::vector<double>(), 44100), std::invalid_argument);
}

TEST(MpmEdgeCase, SmallData)
{
	auto pitch = pitch::mpm(std::vector<double>(1), 44100);
	ASSERT_EQ(-1.0, pitch);
}

TEST(MpmEdgeCase, EmptyAlloc)
{
	EXPECT_THROW(MpmAlloc ma(-1), std::bad_alloc);
}

TEST(YinEdgeCase, EmptyData)
{
	EXPECT_THROW(
	    pitch::mpm(std::vector<double>(), 44100), std::invalid_argument);
}

TEST(YinEdgeCase, TooSmallData)
{
	EXPECT_THROW(
	    pitch::yin(std::vector<double>(1), 44100), std::invalid_argument);
}

TEST(YinEdgeCase, SmallData)
{
	EXPECT_NO_THROW(pitch::yin(std::vector<double>(2), 44100));
}

TEST(YinEdgeCase, EmptyAlloc)
{
	EXPECT_THROW(YinAlloc ya(-1), std::bad_alloc);
}

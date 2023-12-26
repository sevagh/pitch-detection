#include "pitch_detection.h"
#include <gtest/gtest.h>

TEST(MpmEdgeCase, EmptyData)
{
	EXPECT_THROW(
	    pitch::mpm(std::vector<float>(), 44100), std::bad_alloc);
}

TEST(MpmEdgeCase, SmallData)
{
	auto pitch = pitch::mpm(std::vector<float>(2), 44100);
	ASSERT_EQ(-1.0, pitch);
}

TEST(MpmEdgeCase, InvalidAlloc)
{
	EXPECT_THROW(pitch_alloc::Mpm ma(-1), std::length_error);
}

TEST(MpmEdgeCase, EmptyAlloc)
{
	EXPECT_THROW(pitch_alloc::Mpm ma(0), std::bad_alloc);
}

TEST(YinEdgeCase, EmptyData)
{
	EXPECT_THROW(
	    pitch::mpm(std::vector<float>(), 44100), std::bad_alloc);
}

TEST(YinEdgeCase, TooSmallData)
{
	EXPECT_THROW(
	    pitch::yin(std::vector<float>(1), 44100), std::bad_alloc);
}

TEST(YinEdgeCase, SmallData)
{
	EXPECT_NO_THROW(pitch::yin(std::vector<float>(2), 44100));
}

TEST(YinEdgeCase, InvalidAlloc)
{
	EXPECT_THROW(pitch_alloc::Yin ya(-1), std::length_error);
}

TEST(YinEdgeCase, EmptyAlloc)
{
	EXPECT_THROW(pitch_alloc::Yin ma(0), std::bad_alloc);
}

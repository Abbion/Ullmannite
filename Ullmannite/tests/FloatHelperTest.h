#pragma once
#include "gtest/gtest.h"
#include "Utilities/FloatHelper.h"

using namespace Ull;

TEST(FLOAT_TEST, IS_ZERO_FOR_ZERO)
{
    EXPECT_TRUE(IsZero(0.0f));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_ZERO_NEGATIVE)
{
    EXPECT_TRUE(IsZero(-0.0f));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_POSITIVE)
{
    EXPECT_FALSE(IsZero(5.0f));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_NEGATIVE)
{
    EXPECT_FALSE(IsZero(-112.0f));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_QUIET_NAN)
{
    EXPECT_FALSE(IsZero(std::numeric_limits<float>::quiet_NaN()));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_SIGNALING_NAN)
{
    EXPECT_FALSE(IsZero(std::numeric_limits<float>::signaling_NaN()));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_INFINITY)
{
    EXPECT_FALSE(IsZero(std::numeric_limits<float>::infinity()));
}

TEST(FLOAT_TEST, IS_ZERO_FOR_EPSILON)
{
    EXPECT_TRUE(IsZero(std::numeric_limits<float>::epsilon()));
}

TEST(FLOAT_TEST, IS_EQUAL_POSITIVE)
{
    EXPECT_TRUE(IsEqual(23.0f, 23.0f));
}

TEST(FLOAT_TEST, IS_EQUAL_NEGATIVE)
{
    EXPECT_TRUE(IsEqual(-53.0f, -53.0f));
}

TEST(FLOAT_TEST, IS_EQUAL_ZERO)
{
    EXPECT_TRUE(IsEqual(0.0f, 0.0f));
}

TEST(FLOAT_TEST, IS_EQUAL_SIGN_DIFFERENT)
{
    EXPECT_FALSE(IsEqual(53.0f, -53.0f));
}

TEST(FLOAT_TEST, IS_EQUAL_DIFFERENT_POSTIVE)
{
    EXPECT_FALSE(IsEqual(53.0f, 33.2f));
}

TEST(FLOAT_TEST, IS_EQUAL_DIFFERENT_NEGATIVE)
{
    EXPECT_FALSE(IsEqual(-23.1f, -33.2f));
}

TEST(FLOAT_TEST, ROUND_TO_ZERO)
{
    const auto rounded = RoundTo(0.3256f, 0);
    EXPECT_TRUE(IsEqual(rounded, 0.3f));
}

TEST(FLOAT_TEST, ROUND_TO_ONE)
{
    const auto rounded = RoundTo(1.1632f, 1);
    EXPECT_TRUE(IsEqual(rounded, 1.2f));
}

TEST(FLOAT_TEST, ROUND_TO_TWO)
{
    const auto rounded = RoundTo(123.6543f, 2);
    EXPECT_TRUE(IsEqual(rounded, 123.65f));
}

TEST(FLOAT_TEST, ROUND_TO_THREE)
{
    const auto rounded = RoundTo(6.71234f, 3);
    EXPECT_TRUE(IsEqual(rounded, 6.712f));
}

TEST(FLOAT_TEST, ROUND_TO_FOUR)
{
    const auto rounded = RoundTo(65.15789f, 4);
    EXPECT_TRUE(IsEqual(rounded, 65.1579f));
}
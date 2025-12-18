#pragma once
#include "gtest/gtest.h"
#include "Rendering/Utils/RenderHelper.h"
#include "Utilities/FloatHelper.h"

using namespace Ull;

TEST(RGB_TO_HSL_TEST, CONVERT_RED)
{
    const auto rgbInput = glm::vec3(1.0f, 0.0f, 0.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 0.0f) && IsEqual(hslOutput.y, 1.0f) && IsEqual(hslOutput.z, 0.5f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_GREEN)
{
    const auto rgbInput = glm::vec3(0.0f, 1.0f, 0.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 120.0f) && IsEqual(hslOutput.y, 1.0f) && IsEqual(hslOutput.z, 0.5f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_BLUE)
{
    const auto rgbInput = glm::vec3(0.0f, 0.0f, 1.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 240.0f) && IsEqual(hslOutput.y, 1.0f) && IsEqual(hslOutput.z, 0.5f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_CYAN)
{
    const auto rgbInput = glm::vec3(1.0f, 1.0f, 0.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 60.0f) && IsEqual(hslOutput.y, 1.0f) && IsEqual(hslOutput.z, 0.5f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_MAGENTA)
{
    const auto rgbInput = glm::vec3(1.0f, 0.0f, 1.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 300.0f) && IsEqual(hslOutput.y, 1.0f) && IsEqual(hslOutput.z, 0.5f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_YELLOW)
{
    const auto rgbInput = glm::vec3(1.0f, 1.0f, 0.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 60.0f) && IsEqual(hslOutput.y, 1.0f) && IsEqual(hslOutput.z, 0.5f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_BLACK)
{
    const auto rgbInput = glm::vec3(0.0f, 0.0f, 0.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 0.0f) && IsEqual(hslOutput.y, 0.0f) && IsEqual(hslOutput.z, 0.0f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_WHITE)
{
    const auto rgbInput = glm::vec3(1.0f, 1.0f, 1.0f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(hslOutput.x, 0.0f) && IsEqual(hslOutput.y, 0.0f) && IsEqual(hslOutput.z, 1.0f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_HOT_PINK)
{
    const auto rgbInput = glm::vec3(1.0f, 0.4112f, 0.7059f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hslOutput.x, 2), 329.97f) &&
                IsEqual(RoundTo(hslOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hslOutput.z, 2), 0.71f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_HOT_MEDIUM_SEA_GREEN)
{
    const auto rgbInput = glm::vec3(0.2353f, 0.702f, 0.4431f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hslOutput.x, 2), 146.72f) &&
                IsEqual(RoundTo(hslOutput.y, 2), 0.5f) &&
                IsEqual(RoundTo(hslOutput.z, 2), 0.47f));
}

TEST(RGB_TO_HSL_TEST, CONVERT_HOT_ROYAL_BLUE)
{
    const auto rgbInput = glm::vec3(0.2549f, 0.4118f, 0.8824f);
    const auto hslOutput = RgbToHsl(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hslOutput.x, 2), 225.00f) &&
                IsEqual(RoundTo(hslOutput.y, 2), 0.73f) &&
                IsEqual(RoundTo(hslOutput.z, 2), 0.57f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_RED)
{
    const auto hslInput = glm::vec3(0.0f, 1.0f, 0.5f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_GREEN)
{
    const auto hslInput = glm::vec3(120.0f, 1.0f, 0.5f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_BLUE)
{
    const auto hslInput = glm::vec3(240.0f, 1.0f, 0.5f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_CYAN)
{
    const auto hslInput = glm::vec3(60.0f, 1.0f, 0.5f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_MAGENTA)
{
    const auto hslInput = glm::vec3(300.0f, 1.0f, 0.5f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_YELLOW)
{
    const auto hslInput = glm::vec3(60.0f, 1.0f, 0.5f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_BLACK)
{
    const auto hslInput = glm::vec3(0.0f, 0.0f, 0.0f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_WHITE)
{
    const auto hslInput = glm::vec3(0.0f, 0.0f, 1.0f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_HOT_PINK)
{
    const auto hslInput = glm::vec3(330.0f, 1.0f, 0.71f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.42f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.71f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_HOT_MEDIUM_SEA_GREEN)
{
    const auto hslInput = glm::vec3(146.72f, 0.5f, 0.47f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.23f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.7f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.44f));
}

TEST(HSL_TO_RGB_TEST, CONVERT_HOT_ROYAL_BLUE)
{
    const auto hslInput = glm::vec3(225.0, 0.73f, 0.57f);
    const auto rgbOutput = HslToRgb(hslInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.26f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.41f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.88f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_RED)
{
    const auto rgbInput = glm::vec3(1.0f, 0.0f, 0.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_GREEN)
{
    const auto rgbInput = glm::vec3(0.0f, 1.0f, 0.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 120.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_BLUE)
{
    const auto rgbInput = glm::vec3(0.0f, 0.0f, 1.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 240.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_CYAN)
{
    const auto rgbInput = glm::vec3(0.0f, 1.0f, 1.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 180.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_MAGENTA)
{
    const auto rgbInput = glm::vec3(1.0f, 0.0f, 1.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 300.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_YELLOW)
{
    const auto rgbInput = glm::vec3(1.0f, 1.0f, 0.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 60.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_BLACK)
{
    const auto rgbInput = glm::vec3(0.0f, 0.0f, 0.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 0.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_WHITE)
{
    const auto rgbInput = glm::vec3(1.0f, 1.0f, 1.0f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_HOT_PINK)
{
    const auto rgbInput = glm::vec3(1.0f, 0.4112f, 0.7059f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 329.97f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 0.59f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 1.0f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_HOT_MEDIUM_SEA_GREEN)
{
    const auto rgbInput = glm::vec3(0.2353f, 0.702f, 0.4431f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 146.72f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 0.66f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 0.7f));
}

TEST(RGB_TO_HSV_TEST, CONVERT_HOT_ROYAL_BLUE)
{
    const auto rgbInput = glm::vec3(0.2549f, 0.4118f, 0.8824f);
    const auto hsvOutput = RgbToHsv(rgbInput);

    EXPECT_TRUE(IsEqual(RoundTo(hsvOutput.x, 2), 225.0f) &&
                IsEqual(RoundTo(hsvOutput.y, 2), 0.71f) &&
                IsEqual(RoundTo(hsvOutput.z, 2), 0.88f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_RED)
{
    const auto hsvInput = glm::vec3(0.0f, 1.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_GREEN)
{
    const auto hsvInput = glm::vec3(120.0f, 1.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_BLUE)
{
    const auto hsvInput = glm::vec3(240.0f, 1.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_CYAN)
{
    const auto hsvInput = glm::vec3(180.0f, 1.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_MAGENTA)
{
    const auto hsvInput = glm::vec3(300.0f, 1.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_YELLOW)
{
    const auto hsvInput = glm::vec3(60.0f, 1.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_BLACK)
{
    const auto hsvInput = glm::vec3(0.0f, 0.0f, 0.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_WHITE)
{
    const auto hsvInput = glm::vec3(0.0f, 0.0f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 1.0f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_HOT_PINK)
{
    const auto hsvInput = glm::vec3(329.0f, 0.59f, 1.0f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 1.0f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.41f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.71f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_HOT_MEDIUM_SEA_GREEN)
{
    const auto hsvInput = glm::vec3(146.0f, 0.66f, 0.7f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.24f) &&
        IsEqual(RoundTo(rgbOutput.y, 2), 0.7f) &&
        IsEqual(RoundTo(rgbOutput.z, 2), 0.44f));
}

TEST(HSV_TO_RGB_TEST, CONVERT_HOT_ROYAL_BLUE)
{
    const auto hsvInput = glm::vec3(225.0f, 0.71f, 0.88f);
    const auto rgbOutput = HsvToRgb(hsvInput);

    EXPECT_TRUE(IsEqual(RoundTo(rgbOutput.x, 2), 0.26f) &&
                IsEqual(RoundTo(rgbOutput.y, 2), 0.41f) &&
                IsEqual(RoundTo(rgbOutput.z, 2), 0.88f));

}
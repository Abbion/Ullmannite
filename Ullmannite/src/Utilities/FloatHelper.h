#pragma once
#include <limits>
#include <cmath>

namespace Ull
{
    namespace{
        constexpr auto epsilon = std::numeric_limits<float>::epsilon();
    }

    inline bool IsZero(const float a) noexcept {
        if (std::abs(a) <= epsilon)
            return true;
        return false;
    }

    inline bool IsEqual(const float a, const float b) noexcept {
        return IsZero(a - b);
    }

    inline float RoundTo(const float a, const unsigned precision) noexcept {
        const auto ratio = precision > 1 ? std::pow(10, precision) : 10;
        return static_cast<float>(static_cast<std::int64_t>((a * ratio) + 0.5f)) / static_cast<float>(ratio);
    }
}
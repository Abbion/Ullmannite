#pragma once

namespace Ull
{
    enum class State {
        Enable,
        Disable
    };

    enum class ResizeState {
        None,
        Left,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    template<typename T>
    struct Rect {
        constexpr Rect() noexcept = default;
        constexpr Rect(T x, T y, T width, T height) noexcept
            : x{ x }, y{ y }, width{ width }, height{ height } {}

        T x = 0.0f;
        T y = 0.0f;
        T width = 0.0f;
        T height = 0.0f;

        [[nodiscard]] inline bool IsPointInside(T pointX, T pointY) const noexcept {
            return (pointX >= x) && (pointX <= x + width) && (pointY >= y) && (pointY <= y + height);
        }
    };

    typedef Rect<float> RectF;
    typedef Rect<unsigned> RectU;
}
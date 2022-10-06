#pragma once

namespace Ull
{
    class Renderable
    {
    public:
        Renderable() {};
        virtual ~Renderable() {}

        inline void SetVisibility(bool visible) { m_visible = visible; }
        inline bool IsVisible() const { return m_visible; }

        virtual void Render() = 0;

    private:
        bool m_visible{ true };
    };
}
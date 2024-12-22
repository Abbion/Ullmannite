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

        // Add function called render call and in Render use m_visible to call Render call or not
        virtual void Render() = 0;

    private:
        bool m_visible{ true };
    };
}
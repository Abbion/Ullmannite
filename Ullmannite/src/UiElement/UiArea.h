#pragma once
#include "UiElement/UiElement.h"

namespace Ull
{
    class UiArea : public UiElement
    {
    public:
        UiArea(std::string name, glm::uvec2 position, glm::uvec2 size);
        ~UiArea() {}

        void Update() override {};
        void Render() override {};
        void HandleEvent(Event* event) override {};

    private:
        //OpenGL stuff
    };
}
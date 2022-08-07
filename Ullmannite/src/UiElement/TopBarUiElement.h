#include "UiElement/UiElement.h"
#include "glm/glm.hpp"

namespace Ull
{
    class TopBarUiElement : public UiElement;
    {
    public:
        TopBarUiElement();
        ~TopBarUiElement();

        void HandleEvents() override;
        void Update() override;
        void Render() override;

    private:
        
    };
}
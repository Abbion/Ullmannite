#include "Ullpch.h"
#include "UiText.h"

using namespace Ull;

UiText::UiText(std::string name, glm::uvec2 position, glm::uvec2 size, std::wstring text) :
	UiBasicControl{ name, position, size },
	m_text{ text }
{

}
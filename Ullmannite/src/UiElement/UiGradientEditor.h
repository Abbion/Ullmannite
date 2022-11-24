#pragma once
#include "UiElement.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"

namespace Ull
{
	struct GradientPoint
	{
		glm::vec3 m_color;
		uint32_t position;
	};

	class GradientMarker : public UiElement
	{
	public:
		GradientMarker(std::string name, glm::uvec2 position, glm::uvec2 size, glm::vec4 color);
		GradientMarker(GradientMarker&& source);
		~GradientMarker();

		bool IsColorPickerOpened() const { return m_openColorMenu; }
		bool IsMarkerGrabbed() const { return m_markerGrabbed; }

		void SetViewSize(glm::uvec2 size) { m_viewSize = size; }
		void SetViewPos(glm::ivec2 pos) { m_viewPos = pos; }
		void SetMinMaxBounds(int min, int max) { m_minMax.x = min; m_minMax.y = max; }

		void CreateResources() override;

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		bool m_markerGrabbed{ false };
		bool m_openColorMenu{ false };

		bool m_colorMenuJustOpened{ false };
		bool m_pickerOutClick{ false };

		glm::vec4 m_color;
		glm::uvec2 m_viewSize;
		glm::ivec2 m_viewPos;
		glm::ivec2 m_minMax{ 0, 0 };
	};


	class UiGradientEditor : public UiElement
	{
	public:
		UiGradientEditor(std::string name, glm::uvec2 position, glm::uvec2 size);

		void CreateResources() override;

		void SetTransferFunction(NotOwner<TransferFunctionRenderer> transferFunction);

		void SetViewSize(glm::uvec2 size) { m_viewSize = size; }
		void SetViewPos(glm::ivec2 pos) { m_viewPos = pos; }

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		void RenderMarkers();
		void RenderGradient();

		std::vector<GradientMarker> markers;
		glm::uvec2 m_viewSize;
		glm::ivec2 m_viewPos;

		NotOwner<TransferFunctionRenderer> m_transferFunctionRenderer{ nullptr };
	};
}
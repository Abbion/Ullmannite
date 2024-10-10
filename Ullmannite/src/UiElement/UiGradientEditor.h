#pragma once
#include "UiElement.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <glm/glm.hpp>

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

		GradientMarker& operator=(const GradientMarker& source) { return *this; }

		bool IsColorPickerOpened() const { return m_openColorMenu; }
		bool IsMarkerGrabbed() const { return m_markerGrabbed; }
		bool IsMarkerActive() const { return m_markerGrabbed || m_openColorMenu; }
		bool IsMarkerDeleted() const { return m_markerDeleted; }
	
		glm::vec3 GetColor() const { return m_color; }

		void SetViewSize(glm::uvec2 size) { m_viewSize = size; }
		void SetViewPos(glm::ivec2 pos) { m_viewPos = pos; }
		void SetMinMaxBounds(int min, int max) { m_minMax.x = min; m_minMax.y = max; }

		void CreateResources() override;

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		bool m_hovered{ false };
		bool m_markerGrabbed{ false };
		bool m_openColorMenu{ false };

		bool m_colorMenuJustOpened{ false };
		bool m_pickerOutClick{ false };
		bool m_markerDeleted{ false };


		glm::vec4 m_color;
		glm::vec4 m_pointerColor;
		glm::uvec2 m_viewSize;
		glm::ivec2 m_viewPos;
		glm::ivec2 m_minMax{ 0, 0 };

		inline bool PointInMarker(glm::ivec2 point);
	};


	class UiGradientEditor : public UiElement
	{
	public:
		UiGradientEditor(std::string name, glm::uvec2 position, glm::uvec2 size);

		void CreateResources() override;

		void SetTransferFunction(NotOwner<TransferFunctionRenderer> transferFunction);

		void SetViewSize(glm::uvec2 size);
		void SetViewPos(glm::ivec2 pos);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		void RenderMarkers();
		void RenderGradient();

		std::vector<GradientMarker> m_markers;
		glm::uvec2 m_viewSize;
		glm::ivec2 m_viewPos;

		NotOwner<TransferFunctionRenderer> m_transferFunctionRenderer{ nullptr };
	};
}
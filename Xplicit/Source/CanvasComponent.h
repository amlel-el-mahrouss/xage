/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: CanvasComponent.h
 *			Purpose: D2D Canvas.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "Component.h"
#include "DriverD3D11.h"

#include <wrl.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d2d1helper.h>

namespace Xplicit
{
	using float32 = float;
	using float64 = double;

	struct XPLICIT_API Rect
	{
		float32 right;
		float32 bottom;
		float32 left;
		float32 top;
	};
}

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Canvas
{
	class CanvasView;
	class CanvasComponent;

	struct BrushDescriptor;
	struct OutlineDescriptor;

	class CanvasView
	{
	public:
		CanvasView() = default;
		virtual ~CanvasView() = default;

		CanvasView& operator=(const CanvasView&) = default;
		CanvasView(const CanvasView&) = default;

		virtual void operator()(CanvasComponent*) = 0;

	};

	class XPLICIT_API CanvasComponent final : public Component
	{
	public:
		CanvasComponent(Renderer::DX11::DriverSystemD3D11* drv);
		virtual ~CanvasComponent();

		CanvasComponent& operator=(const CanvasComponent&) = default;
		CanvasComponent(const CanvasComponent&) = default;

	public:
		virtual void update() override;

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

		void end_scene();
		void begin_scene();
		void queue(CanvasView* view);
		void transform(const float x = 0, const float y = 0) noexcept;
		void draw_line(const float x1, const float y1, const float x2, const float y2, const float stroke = 1.f) noexcept;
		void draw_rectangle(const Rect rct, const float radiusX = 0, const float radiusY = 0, const float stroke = 1.f);


	private:
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pGhostWhiteBrush;
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1Factory> m_pDirect2dFactory;
		Renderer::DX11::DriverSystemD3D11* m_pDriver;
		std::vector<CanvasView*> m_pViews;

	};
}

#endif
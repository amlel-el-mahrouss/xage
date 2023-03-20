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

#include <wrl.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d2d1helper.h>

namespace Xplicit
{
	struct XPLICIT_API Rect
	{
		float right;
		float bottom;
		float left;
		float top;
	};
}

#ifdef XPLICIT_WINDOWS

namespace Xplicit::XUI
{
	class View;
	class CanvasComponent;

	class View
	{
	public:
		View() = default;
		virtual ~View() = default;

		View& operator=(const View&) = default;
		View(const View&) = default;

		virtual void operator()(CanvasComponent*) {}

	};

	class XPLICIT_API CanvasComponent final : public Component
	{
	public:
		CanvasComponent(HWND hwnd);
		virtual ~CanvasComponent();

		CanvasComponent& operator=(const CanvasComponent&) = default;
		CanvasComponent(const CanvasComponent&) = default;

	public:
		virtual void update() override;

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

		void end_scene();
		void begin_scene();
		void queue(View& view);
		void transform(const float x = 0, const float y = 0) noexcept;
		void draw_line(const float x1, const float y1, const float x2, const float y2, const float stroke = 1.f) noexcept;
		void draw_rectangle(const Rect rct, const float radiusX = 0, const float radiusY = 0, const float stroke = 1.f);


	private:
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pGhostWhiteBrush;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1Factory> m_pDirect2dFactory;

	private:
		HWND m_hwnd;

	private:
		std::vector<View> m_views;

	};
}

#endif
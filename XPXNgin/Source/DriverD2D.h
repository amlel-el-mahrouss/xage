/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverD2D.h
 *			Purpose: D2D renderer.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include "Component.h"

#ifdef XPLICIT_WINDOWS

#include "DriverD3D11.h"

#include <wrl.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d2d1helper.h>

namespace XPX
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

namespace XPX::Renderer::D2D
{
	class DriverSystemD2D;
	class UIView;

	struct BrushDescriptor;
	struct OutlineDescriptor;

	class UIView
	{
	public:
		UIView() = default;
		virtual ~UIView() = default;

		UIView& operator=(const UIView&) = default;
		UIView(const UIView&) = default;

		virtual void operator()(DriverSystemD2D*) = 0;

	};

	class XPLICIT_API DriverSystemD2D final
	{
	public:
		DriverSystemD2D(Renderer::DX11::DriverSystemD3D11* drv);
		~DriverSystemD2D();

		DriverSystemD2D& operator=(const DriverSystemD2D&) = default;
		DriverSystemD2D(const DriverSystemD2D&) = default;

	public:
		void update();
		void end_scene();
		void begin_scene();
		void queue(UIView* view);

		void transform(const float x = 0, 
			const float y = 0) noexcept;

		void draw_line(const float x1,
			const float y1, 
			const float x2, 
			const float y2, 
			const float stroke = 1.f,
			const Color<float> clr = Color<float>(0, 0, 0, 1)) noexcept;

		void draw_rectangle(const Rect rct,
			const float radiusX = 0, 
			const float radiusY = 0, 
			const float stroke = 1.f,
			const Color<float> clr = Color<float>(0, 0, 0, 1)) noexcept;

	public:
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> f_pRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1Factory> f_pDirect2dFactory;
		DX11::DriverSystemD3D11* f_pDriver;
		IDXGISurface* f_pDxgiSurface;
		std::vector<UIView*> m_pViews;
		ID3D11Texture2D* f_pTexture;

	};
}

#endif

#endif
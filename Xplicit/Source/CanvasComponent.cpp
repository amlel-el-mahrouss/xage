/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: CanvasComponent.cpp
 *			Purpose: D2D Canvas.
 *
 * =====================================================================
 */

#include "CanvasComponent.h"

namespace Xplicit::XUI
{
	CanvasComponent::CanvasComponent(HWND hwnd)
		: m_hwnd(hwnd)
	{
		HRESULT hr = S_OK;
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pDirect2dFactory.GetAddressOf());
		XPLICIT_ASSERT(hr == S_OK);

		if (hr == S_OK)
		{
			RECT rc;
			GetClientRect(hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top);

			hr = m_pDirect2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hwnd, size),
				m_pRenderTarget.GetAddressOf());

			if (hr == S_OK)
			{
				hr = m_pRenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::GhostWhite),
					m_pGhostWhiteBrush.GetAddressOf()
				);

				XPLICIT_ASSERT(hr == S_OK);
			}
		}
	}

	CanvasComponent::~CanvasComponent() {}

	void CanvasComponent::update()
	{
		this->begin_scene();

		for (size_t i = 0; i < m_views.size(); ++i)
		{
			m_views[i](this);
		}

		this->end_scene();

		m_views.clear();
	}

	void CanvasComponent::begin_scene()
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->BeginDraw();
	}

	void CanvasComponent::end_scene()
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		HRESULT hr = m_pRenderTarget->EndDraw();
		XPLICIT_ASSERT(SUCCEEDED(hr));
	}

	void CanvasComponent::draw_line(const float x1, const float y1, const float x2, const float y2, const float stroke) noexcept
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x1), y1),
			D2D1::Point2F(static_cast<FLOAT>(x2), y2),
			m_pGhostWhiteBrush.Get(),
			stroke
		);
	}

	void CanvasComponent::draw_rectangle(const Rect rct, const float radiusX, const float radiusY, const float stroke)
	{
		if (!m_pRenderTarget)
			return;

		D2D1_ROUNDED_RECT rect;
		RtlZeroMemory(&rect, sizeof(D2D1_ROUNDED_RECT));

		rect.radiusX = radiusX;
		rect.radiusY = radiusY;

		rect.rect.bottom = rct.bottom;
		rect.rect.left = rct.left;
		rect.rect.top = rct.top;
		rect.rect.right = rct.right;

		m_pRenderTarget->DrawRoundedRectangle(
			&rect,
			m_pGhostWhiteBrush.Get(),
			stroke);
	}

	void CanvasComponent::transform(const float x, const float y) noexcept
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	}

	void CanvasComponent::queue(View& view) { m_views.push_back(view); }

	const char* CanvasComponent::name() noexcept { return ("CanvasComponent"); }
	CanvasComponent::INSTANCE_TYPE CanvasComponent::type() noexcept { return INSTANCE_GUI; }
}
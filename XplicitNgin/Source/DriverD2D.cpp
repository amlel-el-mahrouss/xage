/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: DriverD2D.cpp
 *			Purpose: D2D renderer.
 *
 * =====================================================================
 */

#include "DriverD2D.h"

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Renderer::D2D
{
	DriverSystemD2D::DriverSystemD2D(Renderer::DX11::DriverSystemD3D11* drv)
		: m_pDriver(drv)
	{
		HRESULT hr = S_OK;

		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pDirect2dFactory.GetAddressOf());
		XPLICIT_ASSERT(hr == S_OK);

		if (hr == S_OK)
		{
			Microsoft::WRL::ComPtr<IDXGISurface> surface;
			hr = drv->get().pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)surface.GetAddressOf());

			if (FAILED(hr))
				throw EngineError();

			D2D1_RENDER_TARGET_PROPERTIES renderProp =
				D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_DEFAULT,
					D2D1::PixelFormat(DXGI_FORMAT_R32G32B32_UINT, D2D1_ALPHA_MODE_PREMULTIPLIED),
					0,
					0);

			hr = m_pDirect2dFactory->CreateDxgiSurfaceRenderTarget(
				surface.Get(),
				renderProp,
				m_pRenderTarget.GetAddressOf());

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::GhostWhite),
					m_pGhostWhiteBrush.GetAddressOf()
				);

				if (FAILED(hr))
					throw EngineError();
			}
			else
			{
#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("EngineError: Could not create RenderTarget from DXGISurface!");
#endif // XPLICIT_DEBUG

				throw EngineError();
			}
		}
	}

	DriverSystemD2D::~DriverSystemD2D() {}

	void DriverSystemD2D::update()
	{
		this->begin_scene();

		for (size_t i = 0; i < m_pViews.size(); ++i)
		{
			if (m_pViews[i])
				(*m_pViews[i])(this);
		}

		this->end_scene();

		m_pViews.clear();
	}

	void DriverSystemD2D::begin_scene()
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->BeginDraw();
	}

	void DriverSystemD2D::end_scene()
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		HRESULT hr = m_pRenderTarget->EndDraw();
		XPLICIT_ASSERT(SUCCEEDED(hr));
	}

	void DriverSystemD2D::draw_line(const float x1, const float y1, const float x2, const float y2, const float stroke) noexcept
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

	void DriverSystemD2D::draw_rectangle(const Rect rct, const float radiusX, const float radiusY, const float stroke) noexcept
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

	void DriverSystemD2D::transform(const float x, const float y) noexcept
	{
		if (!m_pRenderTarget)
			return;

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	}

	void DriverSystemD2D::queue(UIView* view) { m_pViews.push_back(view); }
}

#endif
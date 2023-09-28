/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverD2D.cpp
 *			Purpose: D2D renderer.
 *
 * =====================================================================
 */

#include "DriverD2D.h"

#ifdef XPLICIT_WINDOWS

namespace XPX::Renderer::D2D
{
	DriverSystemD2D::DriverSystemD2D(Renderer::DX11::DriverSystemD3D11* drv)
		: f_pDriver(drv), f_pTexture(nullptr), f_pDxgiSurface(nullptr)
	{
		HRESULT hr = S_OK;

		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, f_pDirect2dFactory.GetAddressOf());
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

			hr = f_pDirect2dFactory->CreateDxgiSurfaceRenderTarget(
				surface.Get(),
				renderProp,
				f_pRenderTarget.GetAddressOf());

			DX11::Details::ThrowIfFailed(hr);
		}
	}

	DriverSystemD2D::~DriverSystemD2D() 
	{
		if (f_pTexture)
			f_pTexture->Release();

		if (f_pDxgiSurface)
			f_pDxgiSurface->Release();
	}

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
		if (!f_pRenderTarget)
			return;

		f_pRenderTarget->BeginDraw();
	}

	void DriverSystemD2D::end_scene()
	{
		if (!f_pRenderTarget)
			return;

		f_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		f_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		HRESULT hr = f_pRenderTarget->EndDraw();
		XPLICIT_ASSERT(SUCCEEDED(hr));
	}

	void DriverSystemD2D::draw_line(const float x1,
		const float y1,
		const float x2,
		const float y2, 
		const float stroke,
		const Color<float> clr) noexcept
	{
		if (!f_pRenderTarget)
			return;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;

		HRESULT hr = f_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(clr.R, clr.G, clr.B, clr.A),
			pBrush.GetAddressOf()
		);

		f_pRenderTarget->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x1), y1),
			D2D1::Point2F(static_cast<FLOAT>(x2), y2),
			pBrush.Get(),
			stroke
		);

		pBrush->Release();
	}

	void DriverSystemD2D::draw_rectangle(
		const Rect rct,
		const float radiusX, 
		const float radiusY, 
		const float stroke,
		const Color<float> clr) noexcept
	{
		if (!f_pRenderTarget)
			return;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;

		HRESULT hr = f_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(clr.R, clr.G, clr.B, clr.A),
			pBrush.GetAddressOf()
		);

		D2D1_ROUNDED_RECT rect;
		RtlZeroMemory(&rect, sizeof(D2D1_ROUNDED_RECT));

		rect.radiusX = radiusX;
		rect.radiusY = radiusY;

		rect.rect.bottom = rct.bottom;
		rect.rect.left = rct.left;
		rect.rect.top = rct.top;
		rect.rect.right = rct.right;

		f_pRenderTarget->DrawRoundedRectangle(
			&rect,
			pBrush.Get(),
			stroke);

		pBrush->Release();
	}

	void DriverSystemD2D::transform(const float x, const float y) noexcept
	{
		if (!f_pRenderTarget)
			return;

		f_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	}

	void DriverSystemD2D::queue(UIView* view) { m_pViews.push_back(view); }
}

#endif
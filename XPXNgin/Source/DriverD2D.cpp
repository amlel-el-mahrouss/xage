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

#include <comdef.h>

namespace XPX::Renderer::D2D
{
	DriverSystemD2D::DriverSystemD2D(Renderer::DX11::DriverSystemD3D11* drv)
		: f_pRenderTarget(nullptr), f_pSurface(nullptr)
	{
		HRESULT hr = S_OK;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, f_pDirect2dFactory.GetAddressOf());
		
		if (hr == S_OK)
		{
			D3D11_TEXTURE2D_DESC texDesc{};

			texDesc.ArraySize = 1;
			texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = 0;
			texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			texDesc.Height = RENDERER->get().SwapDesc.BufferDesc.Height;
			texDesc.Width = RENDERER->get().SwapDesc.BufferDesc.Width;
			texDesc.MipLevels = 1;
			texDesc.MiscFlags = 0;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;

			hr = drv->get().pSwapChain->GetBuffer(0, IID_PPV_ARGS(f_pSurface.GetAddressOf()));
			DX11::Details::ThrowIfFailed(hr);

			D2D1_RENDER_TARGET_PROPERTIES renderProp{ D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT) };
			renderProp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
			
			renderProp.dpiX = 96;
			renderProp.dpiY = 96;

			try
			{
				hr = f_pDirect2dFactory->CreateDxgiSurfaceRenderTarget(
					f_pSurface.Get(),
					renderProp,
					f_pRenderTarget.GetAddressOf());

				DX11::Details::ThrowIfFailed(hr);
			}
			catch (Win32Error& _)
			{
#ifdef XPLICIT_DEBUG

				_com_error err(hr);

				DialogHelper::message_box(L"XAGE",
					L"Fatal error!",
					err.ErrorMessage(),
					TD_INFORMATION_ICON,
					TDCBF_OK_BUTTON);

#endif // ifdef XPLICIT_DEBUG

				std::terminate();
			}
		}
	}

	DriverSystemD2D::~DriverSystemD2D() = default;

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
			D2D1::ColorF(clr.R / 255, clr.G / 255, clr.B / 255, clr.A / 255),
			pBrush.GetAddressOf()
		);

		f_pRenderTarget->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x1), y1),
			D2D1::Point2F(static_cast<FLOAT>(x2), y2),
			pBrush.Get(),
			stroke
		);
	}

	void DriverSystemD2D::draw_rectangle(
		const Rect copyFrom,
		const float radiusX, 
		const float radiusY, 
		const float stroke,
		const Color<float> clr) noexcept
	{
		if (!f_pRenderTarget)
			return;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;

		HRESULT hr = f_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(clr.R / 255, clr.G / 255, clr.B / 255, clr.A / 255),
			pBrush.GetAddressOf()
		);

		D2D1_ROUNDED_RECT _rect{ .rect = {.left = copyFrom.L, .top = copyFrom.T, .right = copyFrom.R, .bottom = copyFrom.B },
			.radiusX = radiusX, .radiusY = radiusY};

		f_pRenderTarget->FillRoundedRectangle(
			_rect,
			pBrush.Get());
	}

	void DriverSystemD2D::transform(const float x, const float y) noexcept
	{
		if (!f_pRenderTarget)
			return;

		f_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	}
}

#endif
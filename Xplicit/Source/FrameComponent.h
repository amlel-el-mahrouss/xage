/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: FrameComponent.h
 *			Purpose: D2D Frame.
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
	class FrameComponent;

	class View
	{
	public:
		View() = default;
		virtual ~View() = default;

		View& operator=(const View&) = default;
		View(const View&) = default;

		virtual void operator()(FrameComponent*) {}

	};

	class XPLICIT_API FrameComponent final : public Component
	{
	public:
		FrameComponent(HWND hwnd)
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

		~FrameComponent() {}

		FrameComponent& operator=(const FrameComponent&) = default;
		FrameComponent(const FrameComponent&) = default;

	public:
		virtual void update() override
		{
			this->begin_scene();

			for (size_t i = 0; i < m_views.size(); ++i)
			{
				m_views[i](this);
			}

			this->end_scene();
		}

		virtual const char* name() noexcept override { return ("FrameComponent"); }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		void begin_scene()
		{
			if (!m_pRenderTarget)
				return;

			m_pRenderTarget->BeginDraw();
		}

		void end_scene()
		{
			if (!m_pRenderTarget)
				return;

			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			HRESULT hr = m_pRenderTarget->EndDraw();
			XPLICIT_ASSERT(SUCCEEDED(hr));
		}

		void draw_line(float x1, float y1, float x2, float y2, float stroke = 1.f)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x1), y1),
				D2D1::Point2F(static_cast<FLOAT>(x2), y2),
				m_pGhostWhiteBrush.Get(),
				stroke
			);
		}

		void draw_rectangle(const Rect rct, const float radiusX = 0, const float radiusY = 0, const float stroke = 1.f)
		{
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

		void transform(float x = 0, float y = 0)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
		}

		void queue(View& view)
		{
			m_views.push_back(view);
		}

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
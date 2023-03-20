/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: Bites.h
 *			Purpose: The C++ Windowing library for DirectX
 *
 * =====================================================================
 */

#pragma once

#include "Event.h"
#include "Xplicit.h"
#include "Component.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

#define XPLICIT_MIN_WIDTH  (XPLICIT_DEFAULT_WIDTH)
#define XPLICIT_MIN_HEIGHT (XPLICIT_DEFAULT_HEIGHT)

#ifdef XPLICIT_WINDOWS

#include "DriverD3D11.h"

namespace Xplicit::Bites
{
	class XPLICIT_API Win32_Window final
	{
	public:
		Win32_Window(const char* wndName, const char* wndClass,
			HINSTANCE hInstance)
			: m_traits()
		{
			RtlZeroMemory(&m_traits.WndClass, sizeof(WNDCLASSEXA));

			m_traits.WndClass.cbSize = sizeof(WNDCLASSEXA);
			m_traits.WndClass.lpfnWndProc = Win32_Window::cdecl_window_procedure;
			m_traits.WndClass.lpszClassName = wndClass;
			m_traits.WndClass.hInstance = hInstance;
			m_traits.WndClass.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
			m_traits.WndClass.hIconSm = m_traits.WndClass.hIcon;
			m_traits.WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
			m_traits.WndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
			m_traits.WndClass.lpszMenuName = nullptr;
			m_traits.WndClass.lpszClassName = wndClass;

			RegisterClassExA(&m_traits.WndClass);

			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);

			x = (x - XPLICIT_MIN_WIDTH) / 2;
			y = (y - XPLICIT_MIN_HEIGHT) / 2;

			m_traits.WindowHandle = CreateWindowA(wndClass, 
				wndName, 
				WS_BORDER, 
				x, 
				y, XPLICIT_MIN_WIDTH,
				XPLICIT_MIN_HEIGHT, 
				nullptr, 
				nullptr,
				hInstance, 
				this);

			XPLICIT_ASSERT(m_traits.WindowHandle);

			ShowWindow(m_traits.WindowHandle, SW_SHOW);
			UpdateWindow(m_traits.WindowHandle);
		}

		static LRESULT CALLBACK window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			switch (msg)
			{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}
			}

			return DefWindowProcA(hwnd, msg, wparam, lparam);
		}

		static LRESULT CALLBACK cdecl_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			static Win32_Window* win = nullptr;

			if (win == nullptr && msg == WM_CREATE)
				win = reinterpret_cast<Win32_Window*>(lparam);

			return win ? win->window_procedure(hwnd, msg, wparam, lparam) :
				DefWindowProcA(hwnd, msg, wparam, lparam);
		}

		~Win32_Window()
		{
			if (m_traits.WindowHandle)
				DestroyWindow(m_traits.WindowHandle);
		}

		Win32_Window& operator=(const Win32_Window&) = default;
		Win32_Window(const Win32_Window&) = default;

	public:
		struct Win32Traits
		{
			HWND WindowHandle;
			WNDCLASSEXA WndClass;
		};

		using Traits = Win32Traits;

		int run(Renderer::DX11::DriverSystemD3D11* driver, const Nplicit::Color<float>& clr) noexcept
		{
			MSG msg;
			RtlZeroMemory(&msg, sizeof(MSG));

			int ret = -1;
			bool done = false;

			while (!done)
			{
				if (PeekMessageA(&msg, m_traits.WindowHandle, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				}

				if (msg.message == WM_QUIT)
					done = true;

				driver->begin_scene(clr.A / 255, clr.R / 255, clr.G / 255, clr.B / 255);

				ComponentManager::get_singleton_ptr()->update();
				EventDispatcher::get_singleton_ptr()->update();

				driver->end_scene();

				if (driver->is_closed())
					done = true;
			}

			return ret;
		}

		Traits& get() noexcept { return m_traits; }

	private:
		Win32Traits m_traits;

	};
}
#endif
/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: Bites.h
 *			Purpose: C++ Header only, window class for Direct3D 11.
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
			m_traits.WndClass.cbSize = sizeof(WNDCLASSEXA);
			m_traits.WndClass.lpfnWndProc = Win32_Window::window_procedure;
			m_traits.WndClass.lpszClassName = wndClass;
			m_traits.WndClass.hInstance = hInstance;
			m_traits.WndClass.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
			m_traits.WndClass.hIconSm = m_traits.WndClass.hIcon;
			m_traits.WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
			m_traits.WndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
			m_traits.WndClass.lpszMenuName = nullptr;
			m_traits.WndClass.lpszClassName = wndClass;

			RegisterClassExA(&m_traits.WndClass);

			m_traits.WindowHandle = CreateWindowA(wndClass, wndName, WS_CAPTION, 0, 0, XPLICIT_MIN_WIDTH, XPLICIT_MIN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

			XPLICIT_ASSERT(m_traits.WindowHandle);

			ShowWindow(m_traits.WindowHandle, SW_SHOW);
			UpdateWindow(m_traits.WindowHandle);

			ShowCursor(false);
		}

		static LRESULT CALLBACK window_procedure(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
		{
			switch (umessage)
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

		int run(Renderer::DriverSystemD3D11* driver) noexcept
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

				driver->begin_scene(1.0f, 0.40f, 0.40f, 0.40f);

				ComponentManager::get_singleton_ptr()->update();
				EventDispatcher::get_singleton_ptr()->update();

				driver->end_scene();

				if (driver->should_end())
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
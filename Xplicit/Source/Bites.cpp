/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: Bites.cpp
 *			Purpose: C++ Header only, window class for Direct3D 11.
 *
 * =====================================================================
 */

#include "Bites.h"

namespace Xplicit::Bites
{
#ifdef XPLICIT_WINDOWS
	Win32Window::Win32Window(const char* wndName, const char* wndClass,
		HINSTANCE hInstance)
		: m_traits()
	{
		RtlZeroMemory(&m_traits.WndClass, sizeof(WNDCLASSEXA));

		m_traits.WndClass.cbSize = sizeof(WNDCLASSEXA);
		m_traits.WndClass.lpfnWndProc = Win32Window::cdecl_window_procedure;
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
			y, 
			XPLICIT_MIN_WIDTH,
			XPLICIT_MIN_HEIGHT,
			nullptr,
			nullptr,
			hInstance,
			this);

		XPLICIT_ASSERT(m_traits.WindowHandle);

		ShowWindow(m_traits.WindowHandle, SW_SHOW);
		UpdateWindow(m_traits.WindowHandle);
	}

	LRESULT Win32Window::window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

	LRESULT CALLBACK Win32Window::cdecl_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		static Win32Window* win = nullptr;

		if (win == nullptr && msg == WM_CREATE)
			win = reinterpret_cast<Win32Window*>(lparam);

		return win ? win->window_procedure(hwnd, msg, wparam, lparam) :
			DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	Win32Window::~Win32Window()
	{
		if (m_traits.WindowHandle)
			DestroyWindow(m_traits.WindowHandle);
	}

	Win32Window::Traits& Win32Window::get() noexcept { return m_traits; }

	int Win32Window::run(std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11>& driver, const Nplicit::Color<float>& clr) noexcept
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

			if (!driver->end_scene())
				throw EngineError();

			if (driver->is_closed())
				done = true;
		}

		return ret;
	}
#endif // XPLICIT_WINDOWS
}
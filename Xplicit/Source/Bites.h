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
			HINSTANCE hInstance);

		static LRESULT window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK cdecl_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		~Win32_Window();

		Win32_Window& operator=(const Win32_Window&) = default;
		Win32_Window(const Win32_Window&) = default;

	public:
		struct Win32Traits
		{
			HWND WindowHandle;
			WNDCLASSEXA WndClass;
		};

		using Traits = Win32Traits;

		int run(std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11>& driver, 
			const Nplicit::Color<float>& clr) noexcept;

		Traits& get() noexcept;

	private:
		Win32Traits m_traits;

	};
}
#endif
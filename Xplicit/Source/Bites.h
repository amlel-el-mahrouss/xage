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
#include "DriverOpenGL.h"

#define XPLICIT_GET_X_POS(LPARAM) GET_X_LPARAM(LPARAM)
#define XPLICIT_GET_Y_POS(LPARAM) GET_Y_LPARAM(LPARAM)

namespace Xplicit::Bites
{
	class XPLICIT_API Win32Window final
	{
	public:
		Win32Window(const char* wndName, const char* wndClass,
			HINSTANCE hInstance);

		static LRESULT window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK cdecl_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		~Win32Window();

	public:
		XPLICIT_COPY_DELETE(Win32Window);

	public:
		struct Win32Traits
		{
			HWND WindowHandle;
			WNDCLASSEXA WndClass;
		};

		using Traits = Win32Traits;

		int run(std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11>& driver, 
			const Color<float>& clr) noexcept;

		Traits& get() noexcept;

	private:
		Win32Traits m_traits;

	};

	class XPLICIT_API GLFWWindow final
	{
	public:
		GLFWWindow(const char* windowName);
		~GLFWWindow();

		XPLICIT_COPY_DELETE(GLFWWindow);

	public:
		GLFWwindow* get() noexcept;

	public:
		int run(std::unique_ptr<Xplicit::Renderer::OpenGL::DriverSystemOpenGL>& driver,
			const Color<float>& clr) noexcept;

	private:
		struct GLFWwindow* m_pWindow;

	};
}

#define XPLICIT_MAIN()\
INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)


#endif // XPLICIT_WINDOWS
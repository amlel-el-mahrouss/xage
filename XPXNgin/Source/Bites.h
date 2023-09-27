/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: Bites.h
 *			Purpose: The C++ window library for DirectX/OpenGL/Vk
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

#define XPLICIT_MIN_WIDTH  (XPLICIT_DEFAULT_WIDTH)
#define XPLICIT_MIN_HEIGHT (XPLICIT_DEFAULT_HEIGHT)

#include <glfw3.h>

#ifdef XPLICIT_WINDOWS

#define XPLICIT_GET_X_POS(LPARAM) GET_X_LPARAM(LPARAM)
#define XPLICIT_GET_Y_POS(LPARAM) GET_Y_LPARAM(LPARAM)

#define XPLICIT_MAIN()\
INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)

#include "DriverD3D11.h"

#endif // ifdef XPLICIT_WINDOWS

namespace XPX::Bites
{
#ifdef XPLICIT_WINDOWS
	class XPLICIT_API Win32Window final
	{
	public:
		Win32Window(const char* wnd_name, 
			const char* wnd_class,
			HINSTANCE h_instance);

		static LRESULT window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK cdecl_window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		~Win32Window();
		
		XPLICIT_COPY_DELETE(Win32Window);
		
		struct XPLICIT_API Win32Traits
		{
			HWND WindowHandle;
			WNDCLASSEXA WndClass;
			Renderer::DX11::DriverSystemD3D11* DriverSystem;
		};

		using Traits = Win32Traits;

	public:
		int update() noexcept;
		Traits& get() noexcept;

	private:
		Win32Traits mTraits;
		bool mExit;

	};

#endif // XPLICIT_WINDOWS

	class XPLICIT_API GLFWWindow final
	{
	public:
		GLFWWindow(const char* window_name);
		~GLFWWindow();

		XPLICIT_COPY_DELETE(GLFWWindow);
		
		GLFWwindow* get() const noexcept;
		
	public:
		int update() noexcept;

	private:
		GLFWwindow* mWindow;
		bool mExit;

	};

#ifdef XPLICIT_WINDOWS
	typedef Win32Window BitesWindow;
#else
	typedef GLFWWindow BitesWindow;
#endif // XPLICIT_WINDOWS
}

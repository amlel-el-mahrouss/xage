/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Bites.h
 *			Purpose: The C++ Windowing library for DirectX
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

#define XPLICIT_MIN_WIDTH  (XPLICIT_DEFAULT_WIDTH)
#define XPLICIT_MIN_HEIGHT (XPLICIT_DEFAULT_HEIGHT)

#ifdef XPLICIT_WINDOWS

#include <glfw3.h>

#define XPLICIT_GET_X_POS(LPARAM) GET_X_LPARAM(LPARAM)
#define XPLICIT_GET_Y_POS(LPARAM) GET_Y_LPARAM(LPARAM)

namespace Xplicit::Bites
{
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
		
		struct Win32Traits
		{
			HWND WindowHandle;
			WNDCLASSEXA WndClass;
		};

		using Traits = Win32Traits;

	public:
		int update() noexcept;
		const Traits& get() const noexcept;

	private:
		Win32Traits mTraits;
		bool mExit;

	};

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
}

#define XPLICIT_MAIN()\
INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)


#endif // XPLICIT_WINDOWS
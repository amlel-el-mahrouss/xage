/*
 * =====================================================================
 *
 *			XPXNgin (XplicitRenderer)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: Bites.cpp
 *			Purpose: C++ Header only, window class for Direct3D 11.
 *
 * =====================================================================
 */

#include "GLad.h"
#include "Bites.h"

#ifdef XPLICIT_WINDOWS

namespace XPX::Bites
{
	Win32Window::Win32Window(const char* wndName, const char* wndClass, HINSTANCE hInstance)
		: mTraits(), mExit(false)
	{
		RtlZeroMemory(&mTraits.WndClass, sizeof(WNDCLASSEXA));

		mTraits.WndClass.cbSize = sizeof(WNDCLASSEXA);
		mTraits.WndClass.lpfnWndProc = Win32Window::cdecl_window_procedure;
		mTraits.WndClass.lpszClassName = wndClass;
		mTraits.WndClass.hInstance = hInstance;
		mTraits.WndClass.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
		mTraits.WndClass.hIconSm = mTraits.WndClass.hIcon;
		mTraits.WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		mTraits.WndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		mTraits.WndClass.lpszMenuName = nullptr;
		mTraits.WndClass.lpszClassName = wndClass;

		RegisterClassExA(&mTraits.WndClass);

		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);

		x = (x - XPLICIT_MIN_WIDTH) / 2;
		y = (y - XPLICIT_MIN_HEIGHT) / 2;

		mTraits.WindowHandle = CreateWindowA(wndClass,
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

		XPLICIT_ASSERT(mTraits.WindowHandle);

		ShowWindow(mTraits.WindowHandle, SW_SHOW);
		UpdateWindow(mTraits.WindowHandle);
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
		if (mTraits.WindowHandle)
			DestroyWindow(mTraits.WindowHandle);
	}

	const Win32Window::Traits& Win32Window::get() const noexcept { return mTraits; }

	int Win32Window::update() noexcept
	{
		if (!this->mExit)
		{
			static MSG msg{ 0 };

			if (PeekMessageA(&msg, mTraits.WindowHandle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}

			if (msg.message == WM_QUIT)
				return 1;
		}

		return 0;
	}

	void glfw_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	GLFWWindow::GLFWWindow(const char* windowName)
		: mWindow(nullptr)
	{
		glfwInit();

		glfwSetErrorCallback([](int id, const char* err) { XPLICIT_CRITICAL("ID: " + std::to_string(id) + " What: " + err); throw EngineError(err); });

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mWindow = glfwCreateWindow(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT, windowName, nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);

		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
			
		glViewport(0, 0, XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);

		glfwSetFramebufferSizeCallback(mWindow, glfw_size_callback);
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwTerminate();
	}

	int GLFWWindow::update() noexcept
	{
		if (this->mExit)
		{
			glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
			return 0;
		}

		return 1;
	}

	GLFWwindow* GLFWWindow::get() const noexcept
	{
		return mWindow;
	}
}

#endif // ifdef XPLICIT_WINDOWS
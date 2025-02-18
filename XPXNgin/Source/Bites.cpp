/*
 * =====================================================================
 *
 *			XPXNgin (RigMan)
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: Bites.cpp
 *			Purpose: C++ window library for Direct3D 11 and OpenGL/Vk.
 *
 * =====================================================================
 */

#include "GLad.h"

#include "Bites.h"
#include "Event.h"
#include "Root.h"

#include <windowsx.h>

namespace XPX::Bites
{
#ifdef XPLICIT_WINDOWS
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
		mTraits.WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		mTraits.WndClass.lpszMenuName = nullptr;
		mTraits.WndClass.style = CS_HREDRAW | CS_VREDRAW|  CS_OWNDC;
		mTraits.WndClass.lpszClassName = wndClass;

		RegisterClassExA(&mTraits.WndClass);

		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);

		x = (x - XPLICIT_MIN_WIDTH) / 2;
		y = (y - XPLICIT_MIN_HEIGHT) / 2;

		mTraits.WindowHandle = CreateWindowA(wndClass,
			wndName,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
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
		SetForegroundWindow(mTraits.WindowHandle);
		SetFocus(mTraits.WindowHandle);
	}

	LRESULT Win32Window::window_procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{

		case WM_MOUSEMOVE:
		{
			auto p = MAKEPOINTS(lparam);

			if (Root::get_singleton_ptr()->Keyboard)
			{
				Root::get_singleton_ptr()->Keyboard->mMousePos.X = p.x;
				Root::get_singleton_ptr()->Keyboard->mMousePos.Y = p.y;

#ifdef XPLICIT_DEBUG
				//! debug trace

				fmt::print("X: {}\n", Root::get_singleton_ptr()->Keyboard->mMousePos.X);
				fmt::print("Y: {}\n", Root::get_singleton_ptr()->Keyboard->mMousePos.Y);
#endif // ifdef XPLICIT_DEBUG

			}

			break;
		}
		case WM_RBUTTONDOWN:
		{
			if (Root::get_singleton_ptr()->Keyboard)
			{
				Root::get_singleton_ptr()->Keyboard->mMouseRight.Down = true;

#ifdef XPLICIT_DEBUG
				//! debug trace for mouse stat.

				fmt::print("RightDown: {}\n", Root::get_singleton_ptr()->Keyboard->mMouseRight.Down ? "Yes" : "No");
#endif // ifdef XPLICIT_DEBUG
			}

			break;
		}
		case WM_RBUTTONUP:
		{
			if (Root::get_singleton_ptr()->Keyboard)
			{
				Root::get_singleton_ptr()->Keyboard->mMouseRight.Down = false;

#ifdef XPLICIT_DEBUG
				//! debug trace for mouse stat.

				fmt::print("RightDown: {}\n", Root::get_singleton_ptr()->Keyboard->mMouseRight.Down ? "Yes" : "No");
#endif // ifdef XPLICIT_DEBUG
			}

			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (Root::get_singleton_ptr()->Keyboard)
			{
				Root::get_singleton_ptr()->Keyboard->mMouseLeft.Down = true;

#ifdef XPLICIT_DEBUG
				//! debug trace for mouse stat.

				fmt::print("LeftDown: {}\n", Root::get_singleton_ptr()->Keyboard->mMouseLeft.Down ? "Yes" : "No");
#endif // ifdef XPLICIT_DEBUG
			}

			break;
		}
		case WM_LBUTTONUP:
		{
			if (Root::get_singleton_ptr()->Keyboard)
			{
				Root::get_singleton_ptr()->Keyboard->mMouseLeft.Down = false;

#ifdef XPLICIT_DEBUG
				//! debug trace for mouse stat.

				fmt::print("LeftDown: {}\n", Root::get_singleton_ptr()->Keyboard->mMouseLeft.Down ? "Yes" : "No");
#endif // ifdef XPLICIT_DEBUG
			}

			break;
		}
		case WM_KEYUP:
		{
			if (Root::get_singleton_ptr()->Keyboard)
				Root::get_singleton_ptr()->Keyboard->mKeys[wparam] = false;

			break;
		}
		case WM_KEYDOWN:
		{
			if (Root::get_singleton_ptr()->Keyboard)
				Root::get_singleton_ptr()->Keyboard->mKeys[wparam] = true;

			break;
		}
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
		UnregisterClassA(mTraits.WndClass.lpszClassName, mTraits.WndClass.hInstance);

		if (mTraits.WindowHandle)
			DestroyWindow(mTraits.WindowHandle);
	}

	Win32Window::Traits& Win32Window::get() noexcept { return mTraits; }

	int Win32Window::update() noexcept
	{
		if (!this->mExit)
		{
			static MSG msg{ 0 };

			if (PeekMessage(&msg, mTraits.WindowHandle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}

			if (msg.message == WM_QUIT)
			{
				return (int)msg.message;
			}
		}

		return 0;
	}

#endif // ifdef XPLICIT_WINDOWS

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
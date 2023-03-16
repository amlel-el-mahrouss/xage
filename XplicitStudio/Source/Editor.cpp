/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Editor.cpp
 *			Purpose: Xplicit Studio
 *
 * =====================================================================
 */

/**
* @file
*/

#include <DriverD3D11.h>

INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	try
	{
		Xplicit::open_terminal();

		// HWND, should be borderless.
		Xplicit::Renderer::DriverSystemD3D11 studio_renderer(nullptr);
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::GUI::message_box(L"XplicitStudio", L"C++ Engine Error!\r\nCan't Continue :(", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}
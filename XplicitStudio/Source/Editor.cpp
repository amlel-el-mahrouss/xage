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

namespace Xplicit::Studio
{
	constexpr const wchar_t* XPLICIT_APP_NAME = L"Xplicit Studio";
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
		Xplicit::open_terminal();

		// TODO: HWND, should be borderless.
		Xplicit::Renderer::DriverSystemD3D11 studio_renderer(nullptr);
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::GUI::message_box(Xplicit::Studio::XPLICIT_APP_NAME, L"C++ Engine Error!\r\nCan't Continue :(", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}
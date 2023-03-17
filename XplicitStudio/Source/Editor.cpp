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
#include <Bites.h>

namespace Xplicit::Studio
{
	constexpr const wchar_t* XPLICIT_APP_NAME_WIDE = L"Xplicit Studio";
	constexpr const char* XPLICIT_APP_NAME = "Xplicit Studio";
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		Xplicit::Bites::Win32_Window* window = new Xplicit::Bites::Win32_Window(Xplicit::Studio::XPLICIT_APP_NAME,
																				  "StudioWindowXplicit", hInst);

		std::unique_ptr<Xplicit::Renderer::DriverSystemD3D11> renderer = Xplicit::Renderer::make_driver_system_d3d11(window->get().WindowHandle);
		XPLICIT_ASSERT(renderer);

		return window->run(renderer.get());
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::GUI::message_box(Xplicit::Studio::XPLICIT_APP_NAME_WIDE, L"C++ Engine Error!\r\nCan't Continue :(", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}
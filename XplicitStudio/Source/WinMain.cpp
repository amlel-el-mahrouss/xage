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

#include "BlueprintNode.h"

#include <DriverD3D11.h>
#include <Bites.h>

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Studio
{
	// this is horrendous, but windows imposes us that.
	constexpr const wchar_t* XPLICIT_APP_NAME_WIDE = L"Xplicit Studio";
	constexpr const char* XPLICIT_APP_NAME = "Xplicit Studio";
	constexpr const char* XPLICIT_APP_CLASS = "StudioXplicit";
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		Xplicit::Bites::Win32_Window* window = new Xplicit::Bites::Win32_Window(Xplicit::Studio::XPLICIT_APP_NAME,
																				  Xplicit::Studio::XPLICIT_APP_CLASS, hInst);

		std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11> renderer = Xplicit::Renderer::DX11::make_driver_system_d3d11(window->get().WindowHandle);
		XPLICIT_ASSERT(renderer);

		if (!renderer)
			return 1;

		return window->run(renderer.get(), Xplicit::Nplicit::Color<float>(40, 40, 40, 255));
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::GUI::message_box(L"Something bad happen!\r\nExiting..", L"XplicitNgin", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
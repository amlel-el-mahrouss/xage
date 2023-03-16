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
		Xplicit::Renderer::DriverSystemD3D11 my_render_system(nullptr);
		XPLICIT_INFO(my_render_system.name());

		std::cin.get();
	}
	catch (Xplicit::EngineError err)
	{
		Xplicit::GUI::message_box(L"Uh Oh", L"No adapters we're found");
	}

	return 0;
}
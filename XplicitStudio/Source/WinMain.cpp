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

#include <DriverOpenGL.h>
#include <DriverD3D11.h>
#include <Bites.h>

namespace Xplicit::Studio
{
	class Runner final
	{
	public:
		Runner(HINSTANCE hInst)
		{
			// Search and exit if another Xplicit app is open.
			if (Xplicit::Win32Helpers::find_wnd(L"XplicitEd"))
			{
				Xplicit::Dialog::message_box(L"XplicitEd",
					L"Cannot open more than one instance of the XplicitNgin!",
					MB_OK);

				throw EngineError();
			}

			auto win = new Xplicit::Bites::Win32_Window("XplicitEd", "XplicitEd_Window", hInst);
			auto drv = Xplicit::Renderer::DX11::make_driver_system_d3d11(win->get().WindowHandle);

			auto component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Renderer::DX11::D3D11RenderComponent>();

			component->set(drv.get());
			component->create();

			ExitCode = win->run(drv, Xplicit::Nplicit::Color<float>(40, 40, 40));
		}

		int ExitCode;

	};
}

#ifdef XPLICIT_WINDOWS

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
		Xplicit::Studio::Runner runner(hInst);

		return runner.ExitCode;
#endif
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::Dialog::message_box(L"XplicitEd", L"Something bad happen.. exiting!", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
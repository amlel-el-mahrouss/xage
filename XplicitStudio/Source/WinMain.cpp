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

#include <VideoDriver.h>
#include <Bites.h>

#ifdef XPLICIT_WINDOWS

#define XPLICIT_APP_NAMEA "XplicitEd"
#define XPLICIT_APP_NAMEW L"XplicitEd"

namespace Xplicit::Studio
{
	class Runner final
	{
	public:
		Runner(HINSTANCE hInst)
		{
			// Search and exit if another Xplicit app is open.
			if (Xplicit::Win32Helpers::find_wnd(XPLICIT_APP_NAMEW))
			{
				Xplicit::Dialog::message_box(XPLICIT_APP_NAMEW,
					L"Cannot open more than one instance of the XplicitNgin!",
					MB_OK);

				throw EngineError();
			}


			Xplicit::Bites::Win32Window* win = new Xplicit::Bites::Win32Window(XPLICIT_APP_NAMEA, XPLICIT_APP_NAMEA, hInst);
			std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11> drv = std::make_unique<Xplicit::Renderer::DX11::DriverSystemD3D11>(win->get().WindowHandle);

			win->run(drv, Xplicit::Nplicit::Color<float>(40, 40, 40));

			ExitCode = 0;
		}

		int ExitCode{ 0 };

	};
}

XPLICIT_MAIN()
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		Xplicit::Studio::Runner runner(hInst);
		return runner.ExitCode;
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::Dialog::message_box(L"XplicitEd", L"Something bad happen.. exiting!", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
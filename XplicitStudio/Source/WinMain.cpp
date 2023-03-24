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

#ifdef XPLICIT_WINDOWS

#define XPLICIT_APP_NAME L"XplicitEd"

namespace Xplicit::Studio
{
	class Runner final
	{
	public:
		Runner(HINSTANCE hInst)
		{
			// Search and exit if another Xplicit app is open.
			if (Xplicit::Win32Helpers::find_wnd(XPLICIT_APP_NAME))
			{
				Xplicit::Dialog::message_box(XPLICIT_APP_NAME,
					L"Cannot open more than one instance of the XplicitNgin!",
					MB_OK);

				throw EngineError();
			}

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
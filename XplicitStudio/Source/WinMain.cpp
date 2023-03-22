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

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Studio
{
	constexpr const wchar_t* XPLICIT_APP_NAME = L"Xplicit Studio";
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		// Search and exit if another Xplicit app is open.
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Studio::XPLICIT_APP_NAME))
		{
			Xplicit::Dialog::message_box(Xplicit::Studio::XPLICIT_APP_NAME,
				L"Cannot open more than one instance of the XplicitNgin!",
				MB_OK);

			return 1;
		}



		return 0;
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::Dialog::message_box(L"XplicitNgin", L"Something bad happen.. exiting!", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Main.cpp
 *			Purpose: Main unit.
 *
 * =====================================================================
 */

#include "Application.h"
#include "Uri.h"

#ifdef XPLICIT_WINDOWS

INT32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	try
	{
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Bites::XPLICIT_APP_NAME))
		{
			Xplicit::GUI::message_box(Xplicit::Bites::XPLICIT_APP_NAME, L"Cannot open more than one instance of XplicitNgin!", MB_OK);
			return 1;
		}

		Xplicit::Utils::UriParser uri{ XPLICIT_URI_PROTOCOL };
		uri /= pCmdLine;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
		{
			Xplicit::Utils::UriParser xdp{ XPLICIT_XDP_PROTOCOL };
			xdp /= pCmdLine;
			
			// TODO: XDP Lookup.

			return 1;
		}

		Xplicit::Bites::Application* app = new Xplicit::Bites::Application(uri.get().c_str());
		if (!app)
			throw Xplicit::EngineError();

		while (IRR->run() && Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			// To match the loading's screen colour.
			IRR->getVideoDriver()->beginScene(true, true, irr::video::SColor(40, 40, 40, 40));

			IRR->getSceneManager()->drawAll();
			IRR->getGUIEnvironment()->drawAll();

			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::InstanceManager::get_singleton_ptr()->update();

			IRR->getVideoDriver()->endScene();
		}

		return 0;
	}
	catch (std::runtime_error err)
	{
		MessageBoxA(nullptr, err.what(), "XplicitNgin", MB_OK);
		return -1;
	}
}

#endif

/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Main.cpp
 *			Purpose: Main App unit.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"

#ifdef XPLICIT_WINDOWS

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
		// Search and exit, if another Xplicit app is open.
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Bites::XPLICIT_APP_NAME))
		{
			Xplicit::GUI::message_box(Xplicit::Bites::XPLICIT_APP_NAME, 
				L"Cannot open more than one instance of XplicitNgin!", 
				MB_OK);

			return 1;
		}

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XDP_PROTOCOL };
		uri /= pCmdLine;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
			return 1;

		// create a new app.
		Xplicit::Bites::Application* app = new Xplicit::Bites::Application(uri.get().c_str());

		if (!app)
			throw Xplicit::EngineError();
		
		// and run.
		while (IRR->run() && 
			Xplicit::ComponentManager::get_singleton_ptr() && 
			Xplicit::EventDispatcher::get_singleton_ptr())
		{
			IRR->getVideoDriver()->beginScene(true, true, 
				irr::video::SColor(255, 40, 40, 40));

			IRR->getSceneManager()->drawAll();

			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::ComponentManager::get_singleton_ptr()->update();

			IRR->getVideoDriver()->endScene();
		}

		return 0;
	}
	catch (Xplicit::EngineError& err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO(err.what());
#endif

		Xplicit::GUI::message_box(L"Something bad happen!\r\nExiting..", L"XplicitNgin", MB_ICONASTERISK | MB_OK);
		return -1;
	}
}

#endif
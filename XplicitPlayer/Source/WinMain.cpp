/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: WinMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalHTTPComponent.h"
#include "Application.h"

#include <Bites.h>
#include <codecvt>

#ifdef XPLICIT_WINDOWS

XPLICIT_MAIN()
{
	try
	{
#ifdef XPLICIT_WINDOWS
		WSADATA wsa;
		RtlZeroMemory(&wsa, sizeof(WSADATA));

		Xplicit::init_winsock(&wsa);
#endif

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };
		uri /= pCmdLine;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
			return 1;

		// create a new app.
		Xplicit::Bites::Application* app = new Xplicit::Bites::Application(uri.get().c_str());

		if (!app) throw Xplicit::EngineError("Could not create application context, exiting!");
		
		/* main game loop */
		while (IRR->run() && 
			Xplicit::ComponentManager::get_singleton_ptr() && 
			Xplicit::EventManager::get_singleton_ptr())
		{
			IRR->getVideoDriver()->beginScene(true, true, SColor(255, 40, 40, 40));

			IRR->getSceneManager()->drawAll();
			IRR->getGUIEnvironment()->drawAll();

			Xplicit::EventManager::get_singleton_ptr()->update();
			Xplicit::ComponentManager::get_singleton_ptr()->update();
			
			IRR->getVideoDriver()->endScene();
		}
	}
	catch (Xplicit::EngineError& err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO(err.what());
#endif
		std::wstring exit;

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		exit += L"What: ";
		exit += converter.from_bytes(err.what());
		exit += L"\n";

		Xplicit::DialogHelper::message_box(L"Xplicit Engine", 
			L"Program Failure!", 
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			TDCBF_OK_BUTTON);
	}

	return 0;
}

#endif
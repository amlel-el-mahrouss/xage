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

#include "LocalHTTPManager.h"
#include "NetworkProtocol.h"
#include "Application.h"

#include <XplicitSound.h>
#include <Component.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

#ifdef XPLICIT_WINDOWS

extern void xplicit_load_lua();

XPLICIT_MAIN()
{
	try
	{
		XPLICIT_INIT_COM;

		// Search and exit if another Xplicit player is open.
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Bites::XPLICIT_APP_NAME))
		{
			Xplicit::DialogHelper::message_box(Xplicit::Bites::XPLICIT_APP_NAME, 
				L"Cannot open more than one instance of the Xplicit Ngine", 
				MB_OK);

			return 1;
		}

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

#ifdef XPLICIT_WINDOWS
		WSADATA wsa;
		RtlZeroMemory(&wsa, sizeof(WSADATA));

		Xplicit::init_winsock(&wsa);
#endif // ifdef XPLICIT_WINDOWS

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };

		std::string cmd_line = pCmdLine;
		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
			return 1;

		Xplicit::Bites::Application* application = new Xplicit::Bites::Application(uri);

		if (!application) throw Xplicit::EngineError("Could not create application context, exiting!");

		/* main game loop */
		while (RENDER->run() && 
			Xplicit::ComponentManager::get_singleton_ptr() && 
			Xplicit::EventManager::get_singleton_ptr())
		{
			RENDER->getVideoDriver()->beginScene(true, true, SColor(255, 255, 255, 255));

			RENDER->getSceneManager()->drawAll();
			RENDER->getGUIEnvironment()->drawAll();

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::EventManager::get_singleton_ptr()->update();
			Xplicit::ComponentManager::get_singleton_ptr()->update();
			
			RENDER->getVideoDriver()->endScene();
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

		Xplicit::DialogHelper::message_box(L"Xplicit Client", 
			L"Program Exit", 
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			TDCBF_OK_BUTTON);
	}

	XPLICIT_FINI_COM;
	return 0;
}

#endif
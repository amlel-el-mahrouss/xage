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

#include "LocalToolFactory.h"
#include "Application.h"

#include <Bites.h>
#include <codecvt>

#ifdef XPLICIT_WINDOWS

XPLICIT_MAIN()
{
	try
	{
		// Search and exit if another Xplicit app is open.
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Bites::XPLICIT_APP_NAME))
		{
			Xplicit::DialogHelper::message_box(Xplicit::Bites::XPLICIT_APP_NAME, 
				L"Cannot open more than one instance of the Xplicit Engine!", 
				MB_OK);

			return 1;
		}

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };
		uri /= pCmdLine;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
			return 1;

		// create a new app.
		Xplicit::Bites::Application* app = new Xplicit::Bites::Application(uri.get().c_str());

		if (!app)
			throw Xplicit::EngineError();

		std::atexit([]() -> void {
			auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkComponent>("NetworkComponent");

			if (net)
			{
				Xplicit::NetworkPacket packet;

				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = Xplicit::NETWORK_CMD_STOP;
				net->send(packet);
			}
			});

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
			L"Program Exited", 
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			_TASKDIALOG_COMMON_BUTTON_FLAGS::TDCBF_OK_BUTTON);
	}

	return 0;
}

#endif
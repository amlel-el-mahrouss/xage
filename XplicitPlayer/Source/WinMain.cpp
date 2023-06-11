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
			Xplicit::Dialog::message_box(Xplicit::Bites::XPLICIT_APP_NAME, 
				L"Cannot open more than one instance of the XplicitNgin!", 
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
		
		std::atexit([]() -> void {
			auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkComponent>("NetworkComponent");

			if (net)
			{
				Xplicit::NetworkPacket packet;

				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = Xplicit::NETWORK_CMD_STOP;
				net->send(packet);
			}
		});

		auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkComponent>("NetworkComponent");
		Xplicit::NetworkPacket packet;

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

			packet.cmd[XPLICIT_NETWORK_CMD_ACK] = Xplicit::NETWORK_CMD_ACK;
			net->send(packet);
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

		// message_box(LPCWSTR title, LPCWSTR header, LPCWSTR message, PCWSTR icon, _TASKDIALOG_COMMON_BUTTON_FLAGS buttonFlags)
		Xplicit::Dialog::message_box(L"Xplicit Engine", L"Program Exited (C++ Exception)", exit.c_str(), TD_INFORMATION_ICON, _TASKDIALOG_COMMON_BUTTON_FLAGS::TDCBF_OK_BUTTON);
	}

	return 0;
}

#endif
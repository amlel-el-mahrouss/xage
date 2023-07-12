/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: WinMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"

#include <NetworkProtocol.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <Component.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

#ifdef XPLICIT_WINDOWS

static void XplicitThrowException(Xplicit::EngineError& err);
static void XplicitThrowException(Xplicit::Win32Error& err);

XPLICIT_MAIN()
{
	try
	{
		XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };

		std::string cmd_line = pCmdLine;

		if (cmd_line.empty() ||
			cmd_line.find(XPLICIT_XCONNECT_PROTOCOL) == Xplicit::String::npos)
			return 1;

		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		Xplicit::Utils::InternetProtocolChecker checker;

		if (!checker(uri.get().c_str()))
			return 1;

		std::unique_ptr<Xplicit::Bites::Application> app_ptr = std::make_unique<Xplicit::Bites::Application>(uri);

		if (!app_ptr)
			throw Xplicit::EngineError("XplicitNgine had an fatal error, and couldn't continue; we're sorry!");

		//! The Main Logic and Render loop.
		while (RENDER->run() &&
			Xplicit::ComponentSystem::get_singleton_ptr() &&
			Xplicit::EventSystem::get_singleton_ptr())
		{
			RENDER->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 10, 10, 10));

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::EventSystem::get_singleton_ptr()->update();
			Xplicit::ComponentSystem::get_singleton_ptr()->update();

			RENDER->getSceneManager()->drawAll();
			RENDER->getGUIEnvironment()->drawAll();

			RENDER->getVideoDriver()->endScene();
		}
	}
	catch (Xplicit::EngineError& err)
	{
		XplicitThrowException(err);
	}
	catch (Xplicit::Win32Error& err)
	{
		XplicitThrowException(err);
	}

	XPLICIT_FINI_COM;
	return 0;
}

static void XplicitThrowException(Xplicit::EngineError& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitPlayer",
		L"Program Crash!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

static void XplicitThrowException(Xplicit::Win32Error& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"WHAT: ";
	exit += converter.from_bytes(err.what());
	exit += L"HRESULT: ";
	exit += std::to_wstring(err.hr());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitPlayer",
		L"Program Crash!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

#endif
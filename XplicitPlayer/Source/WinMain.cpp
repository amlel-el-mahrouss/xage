/*
 * =====================================================================
 *
 *			XplicitNgin
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

#include "LocalCameraComponent.h"
#include "Application.h"

#include <NetworkProtocol.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <Component.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

static void xplicit_throw_error(Xplicit::EngineError& err);
static void xplicit_throw_error(Xplicit::Win32Error& err);

#ifdef XPLICIT_WINDOWS

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

		if (cmd_line.empty())
			return 1;

		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
			return 1;

		std::unique_ptr<Xplicit::Bites::Application> app_ptr = std::make_unique<Xplicit::Bites::Application>(uri);

		if (!app_ptr)
			throw Xplicit::EngineError("XplicitNgine had an fatal error, and couldn't continue; we're sorry!");

		/* main game loop */
		while (RENDER->run() &&
			Xplicit::ComponentSystem::get_singleton_ptr() &&
			Xplicit::EventSystem::get_singleton_ptr())
		{
			RENDER->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0x87, 0xCE, 0xEB));

			RENDER->getSceneManager()->drawAll();

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::EventSystem::get_singleton_ptr()->update();
			Xplicit::ComponentSystem::get_singleton_ptr()->update();

			RENDER->getVideoDriver()->endScene();
		}
	}
	catch (Xplicit::EngineError& err)
	{
		xplicit_throw_error(err);
	}
	catch (Xplicit::Win32Error& err)
	{
		xplicit_throw_error(err);
	}

	XPLICIT_FINI_COM;
	return 0;
}

static void xplicit_throw_error(Xplicit::EngineError& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Exit",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

static void xplicit_throw_error(Xplicit::Win32Error& err)
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

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Crash!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

#endif
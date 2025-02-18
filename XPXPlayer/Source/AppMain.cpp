/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: AppMain.cpp
 *			Purpose: Application entrypoint.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "App.h"

#include <NetworkProtocol.h>
#include <DriverD3D11.h>
#include <SceneSystem.h>
#include <SoundDriver.h>
#include <DriverD2D.h>
#include <Component.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

static void XPXThrowException(XPX::EngineError& err);

#ifdef _WIN32
static void XPXThrowException(XPX::Win32Error& err);
#endif

XPLICIT_MAIN()
{
	try
	{
#ifdef _WIN32
		XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
		XPX::open_terminal();
#endif // XPLICIT_DEBUG
#endif
		auto ret = 0;

		// parse the connection uri.
		XPX::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };

		XPX::String cmd_line = pCmdLine;

		if (cmd_line.empty() ||
			cmd_line.find(XPLICIT_XCONNECT_PROTOCOL) == XPX::String::npos)
			return 1;

		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		XPX::Utils::InternetProtocolChecker checker;

		if (!checker(uri.get().c_str()))
			return 1;

		XPX::Bites::ApplicationManager application_manager(uri);

		RENDERER->get().pCamera->set_position(XPX::Vector<XPX::float32>(0, 7.0, -500));

		while (ret != WM_QUIT)
		{
			XPX::Root::get_singleton_ptr()->Renderer->begin_scene(1, 0, 0, 0, true, true);

			XPX::ComponentSystem::get_singleton_ptr()->update();
			XPX::EventSystem::get_singleton_ptr()->update();

			XPX::Audio::XAudioEngine::get_singleton_ptr()->update();

			if (!XPX::Root::get_singleton_ptr()->Renderer->end_scene())
				std::exit(-30);

			XPX::Root::get_singleton_ptr()->Keyboard->reset();
		}
	}
	catch (XPX::EngineError& err)
	{
		XPXThrowException(err);
	}
#ifdef _WIN32
	catch (XPX::Win32Error& err)
	{
		XPXThrowException(err);
	}
#endif


#ifdef _WIN32
	XPLICIT_FINI_COM;
#endif
	return 0;
}

static void XPXThrowException(XPX::EngineError& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"WHAT: ";
	exit += converter.from_bytes(err.what());
	exit += L"\n";

#ifdef _WIN32
	XPX::DialogHelper::message_box(XPX::Bites::XPLICIT_APP_NAME,
		L"FATAL FAILURE!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
#endif
}

#ifdef _WIN32
static void XPXThrowException(XPX::Win32Error& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"WHAT: ";
	exit += converter.from_bytes(err.what());
	exit += L", HRESULT: ";
	exit += std::to_wstring(err.hr());
	exit += L"\n";

	XPX::DialogHelper::message_box(L"XAGE",
		L"FATAL FAILURE!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}
#endif
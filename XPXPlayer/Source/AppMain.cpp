/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: AppMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"
#include "ClientUtils.h"

#include <NetworkProtocol.h>
#include <DriverD3D11.h>
#include <SoundNgin.h>
#include <Component.h>
#include <LuaAPI.h>
#include <ImGUI.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

#ifdef __APPLE__
void XPXMacInitStuff();
#endif

static void XplicitThrowException(XPX::EngineError& err);

#ifdef _WIN32
static void XplicitThrowException(XPX::Win32Error& err);
#endif

#ifdef _WIN32
XPLICIT_MAIN()
#else
int main(int argc, char** argv)
#endif
{
	try
	{
#ifdef _WIN32
        XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
        XPX::open_terminal();
#endif // XPLICIT_DEBUG
#endif

		// parse the connection uri.
		XPX::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };

#ifdef _WIN32
        std::string cmd_line = pCmdLine;
#else
        XPLICIT_ASSERT(argv[1]);
        std::string cmd_line = argv[1];
#endif
		
		if (cmd_line.empty() ||
			cmd_line.find(XPLICIT_XCONNECT_PROTOCOL) == XPX::String::npos)
			return 1;

		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		XPX::Utils::InternetProtocolChecker checker;

		if (!checker(uri.get().c_str()))
			return 1;

#ifdef __APPLE__
        XPXMacInitStuff();
#endif

		std::unique_ptr<XPX::Bites::Application> app_ptr = std::make_unique<XPX::Bites::Application>(uri);

		if (!app_ptr)
			throw XPX::EngineError("XPXPlayer couldn't continue; we're sorry!");

		CAD->getSceneManager()->getParameters()->setAttribute(XPX::COLLADA_CREATE_SCENE_INSTANCES, true);

		CAD->getVideoDriver()->setTextureCreationFlag(XPX::ETCF_ALWAYS_32_BIT, true);

		CAD->getSceneManager()->addLightSceneNode(0, XPX::vector3df(200, 200, 200),
			XPX::SColorf(1.0f, 1.0f, 1.0f), 2000);

		CAD->getSceneManager()->setAmbientLight(XPX::SColorf(0.3f, 0.3f, 0.3f));

		//! The main logic and render loop.
		while (CAD->run())
		{
			CAD->getVideoDriver()->beginScene(true, true, irr::video::SColor(150, 50, 50, 50));

#ifdef _WIN32
			XPX::Audio::XAudioEngine::get_singleton_ptr()->update();
#endif

			CAD->getSceneManager()->drawAll();
			CAD->getGUIEnvironment()->drawAll();

			XPX::ComponentSystem::get_singleton_ptr()->update();
			XPX::EventSystem::get_singleton_ptr()->update();

			CAD->getVideoDriver()->endScene();

			XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("world:RenderOneFrame()");
		}
	}
	catch (XPX::EngineError& err)
	{
		XplicitThrowException(err);
	}
#ifdef _WIN32
	catch (XPX::Win32Error& err)
	{
		XplicitThrowException(err);
	}
#endif


#ifdef _WIN32
	XPLICIT_FINI_COM;
#endif
	return 0;
}

static void XplicitThrowException(XPX::EngineError& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"\n";

#ifdef _WIN32
	XPX::DialogHelper::message_box(L"XPXPlayer",
		L"XPLICIT Couldn't continue!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
#endif
}

#ifdef _WIN32
static void XplicitThrowException(XPX::Win32Error& err)
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

	XPX::DialogHelper::message_box(L"XPLICIT",
		L"Program crash!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}
#endif

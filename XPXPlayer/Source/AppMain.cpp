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

#include "App.h"

#include <NetworkProtocol.h>
#include <DriverD3D11.h>
#include <SceneSystem.h>
#include <SoundNgin.h>
#include <Component.h>
#include <ImGUI.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>
#include <Util.h>

static void XplicitThrowException(XPX::EngineError& err);

#ifdef _WIN32
static void XplicitThrowException(XPX::Win32Error& err);
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

		XPX::Bites::ApplicationManager manager(uri);
		XPX::Renderer::SceneSystem scene;

		scene.f_meshLoader = new XPX::Renderer::SceneLoaderXSD();

		auto nodes = scene.add_scene_node("C:/Users/amlal/XGE/XSD/Sample.xsd");
		auto node = scene.get_scene_node(nodes[0]);
		auto node_teapot = scene.get_scene_node(nodes[1]);

		node->f_pSourceLight = new XPX::Renderer::LightSystem();
		node_teapot->f_pSourceLight = node->f_pSourceLight;

		node->f_pSourceLight->f_vSource.X = 0.1;
		node->f_pSourceLight->f_vSource.Y = 0.1;
		node->f_pSourceLight->f_vSource.Z = 1.0;

		node->f_pSourceLight->f_cColour.R = 1.0;
		node->f_pSourceLight->f_cColour.G = 0.5;
		node->f_pSourceLight->f_cColour.B = 1.0;
		node->f_pSourceLight->f_cColour.A = 1.0;

		node->set_position(XPX::Vector<XPX::float32>(0, 0, 0));

		auto origin = XPX::Vector<float>(0.1, 0, -100);

		RENDERER->get().pCamera->set_position(origin);
		RENDERER->get().pCamera->set_rotation(XPX::Vector<float>(0, 0, 0));

		XPX::float32 rot = 0;

		while (ret != WM_QUIT)
		{
			node->set_rotation(XPX::Quaternion<XPX::float32>(0, -rot, 0));

			rot += 0.01;

			if (rot > 360)
				rot = 0;

			ret = XPX::Root::get_singleton_ptr()->Window->update();

			XPX::Root::get_singleton_ptr()->Renderer->begin_scene(1, 0, 0, 0, true, true);

			scene.start_frame();

			XPX::ComponentSystem::get_singleton_ptr()->update();

			XPX::EventSystem::get_singleton_ptr()->update();

			scene.end_frame();

			XPX::Root::get_singleton_ptr()->Renderer->end_scene();

			XPX::Root::get_singleton_ptr()->Keyboard->reset();
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
	XPX::DialogHelper::message_box(XPX::Bites::XPLICIT_APP_NAME,
		L"XAGE Couldn't continue!",
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

	XPX::DialogHelper::message_box(L"XAGE",
		L"XAGE got some issues!!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}
#endif

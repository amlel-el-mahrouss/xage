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
#include "ClientFX.h"
#include "LocalCameraComponent.h"

#include <NetworkProtocol.h>
#include <DriverD3D11.h>
#include <SoundNgin.h>
#include <Component.h>
#include <LuaAPI.h>
#include <ImGUI.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

static void XplicitThrowException(XPX::EngineError& err);

extern void XplicitLoadClientLua() noexcept;

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
        XPX::String cmd_line = pCmdLine;
#else
        XPX::String cmd_line = argv[1];
#endif

		if (cmd_line == "/DirectXInDev")
		{
			if (MessageBox(nullptr, L"You're going to see a demo of the in-coming D11RS, proceed?", L"DirectX 11 Rendering System.", MB_OKCANCEL) == IDOK)
			{
				using namespace XPX::Renderer::DX11;

				XPX::Bites::Win32Window* win = new XPX::Bites::Win32Window("XPXPlayer (D3D 11)", "XPXPlayerDirectXInDev", hInst);

				XplicitLoadBaseLua();
				XplicitLoadClientLua();

				DriverSystemD3D11* drv11 = new DriverSystemD3D11(win->get().WindowHandle);
				win->get().DriverSystem = drv11;

				RenderComponentD3D11* component_d3d11 = XPX::ComponentSystem::get_singleton_ptr()->add<RenderComponentD3D11>();

				XPLICIT_ASSERT(component_d3d11);

				component_d3d11->push(XPX::Color<float>(0, 0, 0.5, 1));
				component_d3d11->push(XPX::Color<float>(0, 0.5, 0, 1));
				component_d3d11->push(XPX::Color<float>(0.5, 0, 0, 1));

				float num = 0.5f;

				component_d3d11->push(XPX::Vector<float>(0.0f, -num, 0.0f));
				component_d3d11->push(XPX::Vector<float>(num, -num, 0.0f));
				component_d3d11->push(XPX::Vector<float>(-num, -num, 0.0f));

				std::vector<UINT> indices{ 0, 1, 3, 3, 1, 2 };

				for (auto& indice : indices)
				{
					component_d3d11->push(indice);
				}

				component_d3d11->set_driver(drv11);
				component_d3d11->create();

				win->update();

				delete win; // still though, wanna do that. even though windows still does free the pool.
			}
		}
		else
		{
			if (cmd_line.empty() ||
				cmd_line.find(XPLICIT_XCONNECT_PROTOCOL) == XPX::String::npos)
				return 1;

			cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

			uri /= cmd_line;

			XPX::Utils::InternetProtocolChecker checker;

			if (!checker(uri.get().c_str()))
				return 1;

			std::unique_ptr<XPX::Bites::Application> app_ptr = std::make_unique<XPX::Bites::Application>(uri);

			XPLICIT_ASSERT(app_ptr);

			CAD->getSceneManager()->getParameters()->setAttribute(XPX::COLLADA_CREATE_SCENE_INSTANCES, true);
			CAD->getVideoDriver()->setTextureCreationFlag(XPX::ETCF_ALWAYS_32_BIT, true);
			CAD->getVideoDriver()->setTextureCreationFlag(XPX::ETCF_CREATE_MIP_MAPS, true);

			while (CAD->run() && 
				XPX::ComponentSystem::get_singleton_ptr() &&
				XPX::EventSystem::get_singleton_ptr())
			{
				CAD->getVideoDriver()->beginScene(true, true, XPX::ImGUI::ImColor(255, 40, 40, 40));

				CAD->getSceneManager()->drawAll();
				
#ifdef _WIN32
				XPX::Audio::XAudioEngine::get_singleton_ptr()->update();
#endif

				XPX::ComponentSystem::get_singleton_ptr()->update();
				XPX::EventSystem::get_singleton_ptr()->update();

				CAD->getVideoDriver()->endScene();

				XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("world:RenderOneFrame()");

				KEYBOARD->reset();
			}

			CAD->drop();
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

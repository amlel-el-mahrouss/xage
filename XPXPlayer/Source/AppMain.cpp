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

				XPX::Bites::Win32Window* win = new XPX::Bites::Win32Window("XAGE (D3D11)", "XPXPlayerDirectXInDev", hInst);

				DriverSystemD3D11* drv11 = new DriverSystemD3D11(win->get().WindowHandle);
				win->get().DriverSystem = drv11;

				RenderComponentD3D11* component_d3d11 = XPX::ComponentSystem::get_singleton_ptr()->add<RenderComponentD3D11>();

				component_d3d11->set_driver(drv11);

				XPLICIT_ASSERT(component_d3d11);

				component_d3d11->push(XPX::Color<float>(0, 0, 0.5, 1));
				component_d3d11->push(XPX::Color<float>(0, 0.5, 0, 1));
				component_d3d11->push(XPX::Color<float>(0.5, 0, 0, 1));

				float num = 1.0f;

				component_d3d11->push(XPX::Vector<float>(-num, -num, 0.0f));
				component_d3d11->push(XPX::Vector<float>(0.0f, -num, 0.0f));
				component_d3d11->push(XPX::Vector<float>(0.0f, -num, 0.0f));

				std::vector<UINT> indices{ 0, 1, 2 };

				for (auto& indice : indices)
				{
					component_d3d11->push(indice);
				}

				component_d3d11->create();

				auto ret = 0;

				drv11->get().pCamera->set_position(XPX::Vector<float>(0, 0, -0.5));

				while (ret != WM_QUIT)
				{
					ret = win->update();

					drv11->begin_scene(1, 0.2, 0.2, 0.2, true, true);

					XPX::ComponentSystem::get_singleton_ptr()->update();
					XPX::EventSystem::get_singleton_ptr()->update();

					drv11->end_scene();
				}

				delete win; // still though, wanna do that. even though windows still does free the pool.
			}
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
		L"ENGINE Crash!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}
#endif

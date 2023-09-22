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

				XPX::Bites::Win32Window* win = new XPX::Bites::Win32Window("XPXPlayer (D11RS)", "XPXPlayerDirectXInDev", hInst);

				XplicitLoadBaseLua();
				XplicitLoadClientLua();

				DriverSystemD3D11* drv11 = new DriverSystemD3D11(win->get().WindowHandle);

				auto shader_vertex = D3D11ShaderHelper1::make_shader<XPX::Renderer::DX11::XPLICIT_SHADER_TYPE::Vertex>(
					L"C:/Users/amlal/XPXEngine/Shaders/Vertex.hlsl", 
					"VS", 
					drv11);

				auto shader_pixel = D3D11ShaderHelper1::make_shader<XPX::Renderer::DX11::XPLICIT_SHADER_TYPE::Vertex>(
					L"C:/Users/amlal/XPXEngine/Shaders/Vertex.hlsl",
					"VS",
					drv11);

				RenderComponentD3D11* component_d3d11 = XPX::ComponentSystem::get_singleton_ptr()->add<RenderComponentD3D11>();

				XPLICIT_ASSERT(component_d3d11);

				component_d3d11->push_shader(shader_vertex);

				component_d3d11->push(XPX::Color<float>(1, 1, 1, 1));
				component_d3d11->push(XPX::Color<float>(1, 1, 1, 1));
				component_d3d11->push(XPX::Color<float>(1, 1, 1, 1));

				component_d3d11->push(XPX::Vector<float>(0, 0.5, 0));
				component_d3d11->push(XPX::Vector<float>(0.45f, -0.5, 0.0f));
				component_d3d11->push(XPX::Vector<float>(-0.45f, 0.5, 0.0f));

				D3D11_INPUT_ELEMENT_DESC  polygonLayout[2];
				
				RtlZeroMemory(&polygonLayout, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);

				polygonLayout[0].SemanticName = "POSITION";
				polygonLayout[0].SemanticIndex = 0;
				polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				polygonLayout[0].InputSlot = 0;
				polygonLayout[0].AlignedByteOffset = 0;
				polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				polygonLayout[0].InstanceDataStepRate = 0;

				polygonLayout[1].SemanticName = "COLOR";
				polygonLayout[1].SemanticIndex = 0;
				polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				polygonLayout[1].InputSlot = 0;
				polygonLayout[1].AlignedByteOffset = 12;
				polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				polygonLayout[1].InstanceDataStepRate = 0;

				shader_vertex->get().vInputLayouts.push_back(polygonLayout[0]);
				shader_vertex->get().vInputLayouts.push_back(polygonLayout[1]);

				shader_vertex->get().create_input_layout(drv11->get().pDevice.Get());

				component_d3d11->set_driver(drv11);
				component_d3d11->create();

				while (true)
				{
					win->update();

					drv11->begin_scene(1, 0.5, 0.5, 0.5, false, false);

					XPX::ComponentSystem::get_singleton_ptr()->update();
					XPX::EventSystem::get_singleton_ptr()->update();

					drv11->end_scene();
				}

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

#ifdef __APPLE__
			XPXMacInitStuff();
#endif

			std::unique_ptr<XPX::Bites::Application> app_ptr = std::make_unique<XPX::Bites::Application>(uri);

			XPLICIT_ASSERT(app_ptr);

			CAD->getSceneManager()->getParameters()->setAttribute(XPX::COLLADA_CREATE_SCENE_INSTANCES, true);
			CAD->getVideoDriver()->setTextureCreationFlag(XPX::ETCF_ALWAYS_32_BIT, true);

			while (CAD->run())
			{
				CAD->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 135, 206, 235));

#ifdef _WIN32
				XPX::Audio::XAudioEngine::get_singleton_ptr()->update();
#endif

				XPX::ComponentSystem::get_singleton_ptr()->update();
				XPX::EventSystem::get_singleton_ptr()->update();

				CAD->getSceneManager()->drawAll();

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

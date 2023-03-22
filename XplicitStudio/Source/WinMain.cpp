/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Editor.cpp
 *			Purpose: Xplicit Studio
 *
 * =====================================================================
 */

/**
* @file
*/

#include <DriverOpenGL.h>
#include <DriverD3D11.h>
#include <Bites.h>

#ifdef XPLICIT_WINDOWS

#define XPLICIT_APP_NAME L"Xplicit Editor"

namespace Xplicit::Studio
{
	class RunnerDirectX final
	{
	public:
		RunnerDirectX(HINSTANCE hInst)
		{
			// Search and exit if another Xplicit app is open.
			if (Xplicit::Win32Helpers::find_wnd(XPLICIT_APP_NAME))
			{
				Xplicit::Dialog::message_box(XPLICIT_APP_NAME,
					L"Cannot open more than one instance of the XplicitNgin!",
					MB_OK);

				throw EngineError();
			}

			auto win = new Xplicit::Bites::Win32_Window("XplicitEd", "XplicitEd_Window", hInst);
			auto drv = Xplicit::Renderer::DX11::make_driver_system_d3d11(win->get().WindowHandle);

			auto component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Renderer::DX11::D3D11RenderComponent>();

			auto shader = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader< Xplicit::Renderer::DX11::D3D11_SHADER_TYPE::Vertex>(
				L"bin/debug/Vertex.cso",
				"main",
				drv
			);

			auto color = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader< Xplicit::Renderer::DX11::D3D11_SHADER_TYPE::Pixel>(
				L"bin/debug/Color.cso",
				"main",
				drv
			);

			shader->attach(color);

			component->set(shader);

			shader->get().input_layouts.push_back(D3D11_INPUT_ELEMENT_DESC());
			shader->get().input_layouts[0].SemanticName = "POSITION";
			shader->get().input_layouts[0].SemanticIndex = 0;
			shader->get().input_layouts[0].Format = DXGI_FORMAT_R32G32_FLOAT;
			shader->get().input_layouts[0].InputSlot = 0;
			shader->get().input_layouts[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			shader->get().input_layouts[0].AlignedByteOffset = 0;

			shader->get()._CreateInputLayout(drv->get().Device.Get());

			component->push_back(Xplicit::Nplicit::Vector<float>(-0.5f, 0.5f, 0.0f));
			component->push_back(Xplicit::Nplicit::Vector<float>(0.0f, 0.5f, 0.0f));
			component->push_back(Xplicit::Nplicit::Vector<float>(0.5f, -0.5f, 0.0f));

			component->set(drv.get());

			component->create();

			ExitCode = win->run(drv, Xplicit::Nplicit::Color<float>(40, 40, 40));
		}

		int ExitCode;

	};
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
		Xplicit::Studio::RunnerDirectX runner(hInst);

		return runner.ExitCode;
#endif
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::Dialog::message_box(L"XplicitEd", L"Something bad happen.. exiting!", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
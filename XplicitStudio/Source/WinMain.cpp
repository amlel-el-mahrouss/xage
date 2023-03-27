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

#include <VideoDriver.h>
#include <DriverD2D.h>
#include <Bites.h>

#ifdef XPLICIT_WINDOWS

#define XPLICIT_APP_NAME_ASCII "XplicitEd"
#define XPLICIT_APP_NAME_UNICODE L"XplicitEd"

namespace Xplicit::Studio
{
	class Runner final
	{
	public:
		Runner(HINSTANCE hInst)
		{
			// Search and exit if another Xplicit app is open.
			if (Xplicit::Win32Helpers::find_wnd(XPLICIT_APP_NAME_UNICODE))
			{
				Xplicit::Dialog::message_box(XPLICIT_APP_NAME_UNICODE,
					L"Cannot open more than one instance of the XplicitNgin!",
					MB_OK);

				throw EngineError();
			}

			Xplicit::Bites::Win32Window* win = new Xplicit::Bites::Win32Window(XPLICIT_APP_NAME_ASCII, XPLICIT_APP_NAME_ASCII, hInst);
			std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11> drv = std::make_unique<Xplicit::Renderer::DX11::DriverSystemD3D11>(win->get().WindowHandle);

			auto* component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Renderer::DX11::RenderComponentD3D11>();

			component->push(Xplicit::Nplicit::Vector<float>(-1.f, -1.f, 0));
			component->push(Xplicit::Nplicit::Vector<float>(0.f, -1.f, 0));
			component->push(Xplicit::Nplicit::Vector<float>(1.f, -1.f, 0));

			auto* vertex = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader<Xplicit::Renderer::DX11::XPLICIT_SHADER_TYPE::Vertex>(
				L"XplicitStudio/Source/Vertex.hlsl",
				"VS",
				drv);

			auto* color = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader<Xplicit::Renderer::DX11::XPLICIT_SHADER_TYPE::Pixel>(
				L"XplicitStudio/Source/Color.hlsl",
				"PS",
				drv);

			vertex->get().input_layouts.push_back(D3D11_INPUT_ELEMENT_DESC());
			vertex->get().input_layouts.push_back(D3D11_INPUT_ELEMENT_DESC());

			vertex->get().input_layouts[0].SemanticName = "POSITION";
			vertex->get().input_layouts[0].SemanticIndex = 0;
			vertex->get().input_layouts[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			vertex->get().input_layouts[0].InputSlot = 0;
			vertex->get().input_layouts[0].AlignedByteOffset = 0;
			vertex->get().input_layouts[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertex->get().input_layouts[0].InstanceDataStepRate = 0;

			vertex->get().input_layouts[1].SemanticName = "COLOR";
			vertex->get().input_layouts[1].SemanticIndex = 0;
			vertex->get().input_layouts[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			vertex->get().input_layouts[1].InputSlot = 0;
			vertex->get().input_layouts[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			vertex->get().input_layouts[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			vertex->get().input_layouts[1].InstanceDataStepRate = 0;

			vertex->get().create_input_layout(drv->get().pDevice.Get());
			color->get().create_matrix_buffer<Xplicit::Renderer::DX11::Details::CBUFFER>(drv->get().pDevice.Get());

			component->push(color);
			component->push(vertex);

			component->set(drv.get());
			component->create();

			ExitCode = win->run(drv, Xplicit::Nplicit::Color<float>(40, 40, 40));
		}

		int ExitCode{ 0 };

	};
}

XPLICIT_MAIN()
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		Xplicit::Studio::Runner runner(hInst);
		return runner.ExitCode;
	}
	catch (Xplicit::EngineError& err)
	{
		(void)err;
		Xplicit::Dialog::message_box(L"XplicitEd", L"Something bad happen.. exiting!", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
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

#include <DriverD3D11.h>
#include <Bites.h>

#ifdef XPLICIT_WINDOWS

namespace Xplicit::Studio
{
	constexpr const wchar_t* XPLICIT_APP_NAME_WIDE = L"Xplicit Studio";
	constexpr const char* XPLICIT_APP_NAME = "Xplicit Studio";
	constexpr const char* XPLICIT_APP_CLASS = "XplicitStudio";
}

INT32 WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int nCmdShow)
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		// Search and exit if another Xplicit app is open.
		if (Xplicit::Win32Helpers::find_wnd(Xplicit::Studio::XPLICIT_APP_NAME_WIDE))
		{
			Xplicit::GUI::message_box(Xplicit::Studio::XPLICIT_APP_NAME_WIDE,
				L"Cannot open more than one instance of the XplicitNgin!",
				MB_OK);

			return 1;
		}

		Xplicit::Bites::Win32_Window* window = new Xplicit::Bites::Win32_Window(Xplicit::Studio::XPLICIT_APP_NAME,
																				  Xplicit::Studio::XPLICIT_APP_CLASS, hInst);

		std::unique_ptr<Xplicit::Renderer::DX11::DriverSystemD3D11> renderer = Xplicit::Renderer::DX11::make_driver_system_d3d11(window->get().WindowHandle);
		XPLICIT_ASSERT(renderer);

		if (!renderer)
			return 1;

		std::string entrypoint = "main";

		auto vertex = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader<
			Xplicit::Renderer::DX11::D3D11_SHADER_TYPE::Vertex>(L"bin/debug/Vertex.cso",
				entrypoint.c_str(),
				renderer);

		auto color = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader<
			Xplicit::Renderer::DX11::D3D11_SHADER_TYPE::Pixel>(L"bin/debug/Color.cso",
				entrypoint.c_str(),
				renderer);

		vertex->attach(color);

		auto component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Renderer::RenderComponent>();

		vertex->get().input_layouts.push_back(D3D11_INPUT_ELEMENT_DESC());
		vertex->get().input_layouts.push_back(D3D11_INPUT_ELEMENT_DESC());

		vertex->get().input_layouts[0].SemanticName = "POSITION";
		vertex->get().input_layouts[0].SemanticIndex = 0;
		vertex->get().input_layouts[0].Format = DXGI_FORMAT_R32G32_FLOAT;
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

		vertex->get()._CreateInputLayout(renderer->get().Device.Get());

		component->set(vertex);

		component->push_back(Xplicit::Nplicit::Vector<float>(0.0f, 0.5f, 0.0f));
		component->push_back(Xplicit::Nplicit::Vector<float>(0.5f, -0.5f, 0.0f));
		component->push_back(Xplicit::Nplicit::Vector<float>(-0.5f, -0.5f, 0.0f));

		component->set(renderer.get());
		component->create();

		return window->run(renderer, Xplicit::Nplicit::Color<float>(20, 20, 20, 255));
	}
	catch (Xplicit::EngineError& err)
	{
		Xplicit::GUI::message_box(L"XplicitNgin", L"Something bad happen.. exiting!", MB_ICONASTERISK | MB_OK);
	}

	return 0;
}

#endif
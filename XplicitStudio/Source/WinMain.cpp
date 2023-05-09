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

#include <DriverVulkan.h>
#include <DriverD2D.h>
#include <Bites.h>
#include <codecvt>

#ifdef XPLICIT_WINDOWS

#define XPLICIT_APP_NAME_ASCII "XplicitEd"

namespace Xplicit::Studio
{
	int ExitCode{ 0 };

	class Runner final
	{
	public:
		explicit Runner(HINSTANCE hInst)
		{
			std::unique_ptr<Xplicit::Bites::Win32Window> wnd = std::make_unique<Xplicit::Bites::Win32Window>("XplicitEd", "XplicitEd", hInst);
			auto drv = std::make_unique<Xplicit::Renderer::DX11::DriverSystemD3D11>(wnd->get().WindowHandle);

			auto* component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Renderer::DX11::RenderComponentD3D11>();

			component->push(Xplicit::Vector<float>(-1.f, -1.f, 0));
			component->push(Xplicit::Vector<float>(0.f, -1.f, 0));
			component->push(Xplicit::Vector<float>(1.f, -1.f, 0));

			auto* vertex = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader<Xplicit::Renderer::DX11::XPLICIT_SHADER_TYPE::Vertex>(
				L"XplicitStudio/Source/Vertex.hlsl",
				"VS",
				drv);

			auto* color = Xplicit::Renderer::DX11::D3D11ShaderHelper1::make_shader<Xplicit::Renderer::DX11::XPLICIT_SHADER_TYPE::Pixel>(
				L"XplicitStudio/Source/Color.hlsl",
				"PS",
				drv);

			component->push(color);
			component->push(vertex);

			component->set(drv.get());
			component->create();

			ExitCode = wnd->run(drv, Color<float>(40, 57, 22, 255));
		}

	};
}

XPLICIT_MAIN()
{
	try
	{
		Xplicit::open_terminal();
		Xplicit::Studio::Runner runner(hInst);
	}
	catch (Xplicit::EngineError& err)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		std::wstring exit = L"Exit Code: ";
		exit += std::to_wstring(Xplicit::Studio::ExitCode);
		exit += L".\n";
		exit += L"What: ";
		exit += converter.from_bytes(err.what());
		exit += L"\n";

		// message_box(LPCWSTR title, LPCWSTR header, LPCWSTR message, PCWSTR icon, _TASKDIALOG_COMMON_BUTTON_FLAGS buttonFlags)
		Xplicit::Dialog::message_box(L"XplicitEd", L"Program Exited (C++ Exception)", exit.c_str(), TD_INFORMATION_ICON, _TASKDIALOG_COMMON_BUTTON_FLAGS::TDCBF_OK_BUTTON);

	}

	return 0;
}

#endif
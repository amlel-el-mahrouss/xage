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
			auto wnd = new Xplicit::Bites::GLFWWindow("XplicitEd");
			std::unique_ptr<Xplicit::Renderer::Vk::DriverSystemVulkan> drv = std::make_unique<Xplicit::Renderer::Vk::DriverSystemVulkan>(glfwGetWin32Window(wnd->get()));
			// ExitCode = wnd->run(drv, Xplicit::Color<float>(255, 255, 255, 15));
		}

	};
}

XPLICIT_MAIN()
{
	try
	{
#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

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
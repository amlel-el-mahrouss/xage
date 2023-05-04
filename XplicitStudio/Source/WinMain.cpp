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
			auto drv = std::make_unique<Xplicit::Renderer::Vk::DriverSystemVulkan>(wnd);
			while (getchar());
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
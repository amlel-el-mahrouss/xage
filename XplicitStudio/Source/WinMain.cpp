/*
 * =====================================================================
 *
 *			XplicitStudio
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: WinMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

#include <Xplicit.h>

#include <NetworkProtocol.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <Component.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

static void XplicitThrowException(Xplicit::EngineError& err);

#ifdef XPLICIT_WINDOWS

static void XplicitThrowException(Xplicit::Win32Error& err);

XPLICIT_MAIN()
{
	try
	{
		XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

	}
	catch (Xplicit::EngineError& err)
	{
		XplicitThrowException(err);
	}
	catch (Xplicit::Win32Error& err)
	{
		XplicitThrowException(err);
	}

	XPLICIT_FINI_COM;
}

static void XplicitThrowException(Xplicit::EngineError& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Exit",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

static void XplicitThrowException(Xplicit::Win32Error& err)
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

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Crash!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

#endif
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
#include <Nplicit.h>

#include <NetworkProtocol.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <Component.h>
#include <ImGUI.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>
#include <Root.h>

#ifdef XPLICIT_WINDOWS

static void XplicitThrowException(Xplicit::EngineError& err);
static void XplicitThrowException(Xplicit::Win32Error& err);

XPLICIT_MAIN()
{
	try
	{
		XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

		Xplicit::Root::get_singleton_ptr()->set(irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(1920, 1080), 32U, false, true, true));
		Xplicit::Root::get_singleton_ptr()->set(new Xplicit::InputReceiver());

		RENDER->setWindowCaption(L"XPLICIT Studio");
		RENDER->setEventReceiver(KB);

		Xplicit::ImGUI::UIButton Insert(L"FILE");

		Insert->W = 192;
		Insert->H = 40;

		Insert->BackgroundColor.setRed(0x1C);
		Insert->BackgroundColor.setGreen(0x1C);
		Insert->BackgroundColor.setBlue(0x1C);
		Insert->BackgroundColor.setAlpha(0xFF);

		Xplicit::ImGUI::UIButton Edit(L"EDIT");

		Edit->W = 192;
		Edit->H = 40;
		Edit->X = 192;

		Edit->BackgroundColor.setRed(0x1C);
		Edit->BackgroundColor.setGreen(0x1C);
		Edit->BackgroundColor.setBlue(0x1C);
		Edit->BackgroundColor.setAlpha(0xFF);

		Xplicit::ImGUI::UIButton View(L"VIEW");

		View->W = 192;
		View->H = 40;
		View->X = 192 * 2;

		View->BackgroundColor.setRed(0x1C);
		View->BackgroundColor.setGreen(0x1C);
		View->BackgroundColor.setBlue(0x1C);
		View->BackgroundColor.setAlpha(0xFF);

		//! The Main Logic and Render loop.
		while (RENDER->run() &&
			Xplicit::ComponentSystem::get_singleton_ptr() &&
			Xplicit::EventSystem::get_singleton_ptr())
		{
			RENDER->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 10, 10, 10));

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::EventSystem::get_singleton_ptr()->update();
			Xplicit::ComponentSystem::get_singleton_ptr()->update();

			Edit.update();
			View.update();
			Insert.update();

			RENDER->getSceneManager()->drawAll();
			RENDER->getGUIEnvironment()->drawAll();

			RENDER->getVideoDriver()->endScene();
		}
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

	Xplicit::DialogHelper::message_box(L"XplicitStudio",
		L"Program Crash!",
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
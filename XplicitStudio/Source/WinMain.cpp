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
#include <BaseLuaAPI.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <Component.h>
#include <ImGUI.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>
#include <Root.h>

#include "PropertyGrid.h"

#ifdef XPLICIT_WINDOWS

static void XplicitThrowException(Xplicit::EngineError& err);
static void XplicitThrowException(Xplicit::Win32Error& err);

int64_t XPLICIT_FILE_COOLDOWN = 18000;
bool XPLICIT_FILE_REQ = false;

using namespace Xplicit;

XPLICIT_MAIN()
{

	try
	{
		XPLICIT_INIT_COM;

		InitPropertyGrid(hInst);


		XplicitLoadBaseLua();

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

		auto* kb = new Xplicit::InputReceiver();

		Xplicit::Root::get_singleton_ptr()->set(irr::createDevice(irr::video::EDT_OPENGL,
			irr::core::dimension2du(1280, 720),
			32U,
			false,
			true,
			true,
			kb));

		KB = kb;

		RENDER->setWindowCaption(L"Xplicit Studio");
		
		Xplicit::ImGUI::UIWindow ribbon;
		
		Xplicit::ImGUI::UIButton file(L"Menu");

		file.get()->W = 122;
		file.get()->H = 30;

		file.LeftClicked = []() {
			XPLICIT_FILE_REQ = !XPLICIT_FILE_REQ;
			XPLICIT_FILE_COOLDOWN = 18000;
		};

		Xplicit::ImGUI::UIButton exit_but(L"Quit Studio");

		exit_but.get()->Y = 33;

		exit_but.LeftClicked = []() {
			XPLICIT_FILE_REQ = false;
			std::exit(0);
		};

		exit_but.get()->W = 122;
		exit_but.get()->H = 30;

		ribbon.pos(0, 0);
		ribbon.size(1280, 140, 30);

		Xplicit::ImGUI::UIWindow worldProperties;

		worldProperties.set("World");
		worldProperties.pos(970, 140);
		worldProperties.size(310, 245, 23);

		Xplicit::ImGUI::UIWindow classProperties;

		classProperties.pos(970, 385);
		classProperties.set("Properties");
		classProperties.size(310, 312, 23);

		XPLICIT_GET_DATA_DIR(XPLICIT_STUDIO_DIRECTORY);
		Xplicit::String skydome_path = XPLICIT_STUDIO_DIRECTORY;
		skydome_path += "/Contents/Studio/Skydome.obj";

		irr::scene::IMeshSceneNode* Skydome = RENDER->getSceneManager()->addMeshSceneNode(RENDER->getSceneManager()->getMesh(skydome_path.c_str()));
		irr::scene::ICameraSceneNode* Cam = RENDER->getSceneManager()->addCameraSceneNodeMaya();

		Skydome->setPosition(irr::core::vector3df(0, 0, 0));

		//! The Main Logic and Render loop.
		while (RENDER->run() &&
			Xplicit::ComponentSystem::get_singleton_ptr() &&
			Xplicit::EventSystem::get_singleton_ptr())
		{
			RENDER->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::EventSystem::get_singleton_ptr()->update();
			Xplicit::ComponentSystem::get_singleton_ptr()->update();

			RENDER->getGUIEnvironment()->drawAll();
			RENDER->getSceneManager()->drawAll();

			ribbon.update();

			file.update();

			worldProperties.update();
			classProperties.update();

			if (XPLICIT_FILE_REQ)
			{
				exit_but.update();
		
				if (XPLICIT_FILE_COOLDOWN)
					--XPLICIT_FILE_COOLDOWN;
			}
		
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

	Xplicit::DialogHelper::message_box(L"Xplicit Studio",
		L"XPLICIT Couldn't continue!",
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
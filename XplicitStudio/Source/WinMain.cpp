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

#ifdef XPLICIT_WINDOWS

static void XplicitThrowException(Xplicit::EngineError& err);
static void XplicitThrowException(Xplicit::Win32Error& err);

XPLICIT_MAIN()
{
	try
	{
		XPLICIT_INIT_COM;

		XplicitLoadBaseLua();

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

		Xplicit::Root::get_singleton_ptr()->set(irr::createDevice(irr::video::EDT_OPENGL, 
			irr::core::dimension2du(1280, 720),
			32U, 
			false,
			true, 
			true));

		Xplicit::Root::get_singleton_ptr()->set(new Xplicit::InputReceiver());

		RENDER->setWindowCaption(L"XPLICIT");
		RENDER->setEventReceiver(KB);

		Xplicit::ImGUI::UIWindow ribbon;
		
		ribbon.pos(0, 0);
		ribbon.size(1280, 140, 30);

		Xplicit::ImGUI::UIButton butEdit(L"EDIT");
		butEdit->W = 169;
		butEdit->H = 30;

		butEdit->X = 169;

		Xplicit::ImGUI::UIButton butInsert(L"INSERT");
		butInsert->W = 169;
		butInsert->H = 30;

		butInsert->X = 169 * 2;

		Xplicit::ImGUI::UIButton butImport(L"IMPORT");
		butImport->W = 169;
		butImport->H = 30;

		butImport->X = 169 * 3;

		Xplicit::ImGUI::UIButton butExport(L"EXPORT");
		butExport->W = 169;
		butExport->H = 30;

		butExport->X = 169 * 3.5;

		Xplicit::ImGUI::UIButton butPublish(L"PUBLISH");
		butPublish->W = 169;
		butPublish->H = 30;

		butExport->X = 169 * 4;

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

			worldProperties.update();
			classProperties.update();

			butEdit.update();
			butInsert.update();
			butImport.update();
			butExport.update();
			butPublish.update();

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
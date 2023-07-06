/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: WinMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalCameraComponent.h"
#include "Application.h"

#include <NetworkProtocol.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <Component.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

static void xplicit_throw_error(Ogre::InvalidParametersException& err);
static void xplicit_throw_error(Ogre::RuntimeAssertionException& err);
static void xplicit_throw_error(Xplicit::EngineError& err);
static void xplicit_throw_error(Xplicit::Win32Error& err);

//! does the necessary things in order to access the Ogre3D_ fields.
static void xplicit_setup_ogre3d()
{
	Ogre::Root* root = Xplicit::Root::get_singleton_ptr()->getRoot();
	Ogre::SceneManager* scnMgr = root->createSceneManager();

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// without light we would just get a black screen, this makes sense, as nothing in refracted to our eye.
	Ogre::Light* light = scnMgr->createLight("MainLight");

	Xplicit::Root::get_singleton_ptr()->Ogre3D_Light = scnMgr->getRootSceneNode()->createChildSceneNode();

	Xplicit::Root::get_singleton_ptr()->Ogre3D_Light->setPosition(0, 10, 15);
	Xplicit::Root::get_singleton_ptr()->Ogre3D_Light->attachObject(light);

	Xplicit::Root::get_singleton_ptr()->Ogre3D_Scene = scnMgr;
	Xplicit::Root::get_singleton_ptr()->Ogre3D_RTSS = shadergen;
	Xplicit::Root::get_singleton_ptr()->Ogre3D_Window = Xplicit::Root::get_singleton_ptr()->getRenderWindow();
}

#ifdef XPLICIT_WINDOWS

XPLICIT_MAIN()
{
	try
	{
		XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif // XPLICIT_DEBUG

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };

		std::string cmd_line = pCmdLine;

		if (cmd_line.empty() ||
			cmd_line.find(XPLICIT_XCONNECT_PROTOCOL) == std::string::npos)
			return 1;

		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		if (inet_addr(uri.get().c_str()) == XPLICIT_INVALID_ADDR)
			return 1;

		Xplicit::Root::get_singleton_ptr()->initApp();
		Xplicit::Root::get_singleton_ptr()->addInputListener(new Xplicit::OgreInputListener());

		xplicit_setup_ogre3d();

		std::unique_ptr<Xplicit::Bites::Application> pApp = std::make_unique<Xplicit::Bites::Application>(uri);

		if (!pApp) 
			throw Xplicit::EngineError("XplicitNgine had an fatal error, and couldn't continue; we're sorry!");

		Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::LocalCameraComponent>("Camera");

		/* main game loop */
		while (Xplicit::ComponentSystem::get_singleton_ptr() && 
			Xplicit::EventSystem::get_singleton_ptr())
		{
			if (Xplicit::Root::get_singleton_ptr()->getRenderWindow()->isClosed()) break;

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::EventSystem::get_singleton_ptr()->update();
			Xplicit::ComponentSystem::get_singleton_ptr()->update();

			Xplicit::Root::get_singleton_ptr()->getRenderWindow()->update();
		}
	}
	catch (Xplicit::EngineError& err)
	{
		xplicit_throw_error(err);
	}
	catch (Xplicit::Win32Error& err)
	{
		xplicit_throw_error(err);
	}
	catch (Ogre::RuntimeAssertionException& err)
	{
		xplicit_throw_error(err);
	}
	catch (Ogre::InvalidParametersException& err)
	{
		xplicit_throw_error(err);
	}

	Xplicit::Root::get_singleton_ptr()->closeApp();

	XPLICIT_FINI_COM;
	return 0;
}

static void xplicit_throw_error(Xplicit::EngineError& err)
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

static void xplicit_throw_error(Xplicit::Win32Error& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"HRESULT: ";
	exit += std::to_wstring(err.hr());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Exit",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

static void xplicit_throw_error(Ogre::InvalidParametersException& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"Descrption: ";
	exit += converter.from_bytes(err.getFullDescription());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Exit",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

static void xplicit_throw_error(Ogre::RuntimeAssertionException& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"Descrption: ";
	exit += converter.from_bytes(err.getFullDescription());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitNgine",
		L"Program Exit",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}

#endif
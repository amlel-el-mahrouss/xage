/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Application.cpp
 *			Purpose: Application Framework
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"

namespace Xplicit::Player
{
	dimension2du XPLICIT_DIM = dimension2du(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	static void open_prebuilt_dialogs()
	{
		XPLICIT_GET_DATA_DIR(dir);

		String prebuilt = dir;
		prebuilt += "\\Textures\\PreBuiltDialogs.zip";

		auto res = IRR->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true);
		XPLICIT_ASSERT(res);

		if (!res)
			throw std::runtime_error("Missing dialogs! This pack are needed for XplicitPlayer to work.");
	}

	Application::Application(const char* dns)
		: mSettings(), mWsa(), mPath("")
	{
		this->setup();

		ApplicationContext::get_singleton_ptr()->Keyboard->get_layout();

		XPLICIT_GET_DATA_DIR(tmp);
		mPath += tmp;

		XPLICIT_ASSERT(!mPath.empty());

#ifdef XPLICIT_WINDOWS
		Xplicit::init_winsock(&mWsa);
#endif

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		auto loadingComponent = ComponentManager::get_singleton_ptr()->add<Player::SplashScreenComponent>();
		XPLICIT_ASSERT(loadingComponent);

		if (loadingComponent)
			loadingComponent->connect(dns);
	}

	Application::~Application() {}

	void Application::setup()
	{
		auto singleton = Xplicit::ApplicationContext::get_singleton_ptr();

		Xplicit::ApplicationContext::get_singleton_ptr()->set(new Xplicit::InputReceiver());

		singleton->set(
			irr::createDevice(
				irr::video::EDT_DIRECT3D9,
				Xplicit::Player::XPLICIT_DIM,
				32U,
				false,
				true,
				false,
				KB
			)
		);

		open_prebuilt_dialogs();

		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);

		IRR->setWindowCaption(Xplicit::Bites::XPLICIT_APP_NAME);
	}

	Application::SettingsManager::SettingsManager()
		: mSettings()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettings = dat;
		mSettings += "ClientSettings.dat";
	}

	Application::SettingsManager::~SettingsManager() = default;

	void Application::SettingsManager::write(Application::SettingsManager::Traits& traits)
	{
		
	}

	void Application::SettingsManager::read(Application::SettingsManager::Traits& traits)
	{
		
	}
}
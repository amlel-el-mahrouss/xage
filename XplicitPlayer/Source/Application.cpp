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
		prebuilt += "\\Textures\\DefaultSkin.zip";

		if (!IRR->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true))
			throw std::runtime_error("Missing dialogs! This pack are needed for XplicitPlayer to work.");
	}

	Application::Application(const char* dns)
		: mSettings(), mWsa(), mPath("")
	{
		this->create_context();

		XPLICIT_GET_DATA_DIR(tmp);
		mPath += tmp;

		XPLICIT_ASSERT(!mPath.empty());

#ifdef XPLICIT_WINDOWS
		Xplicit::init_winsock(&mWsa);
#endif

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		auto splashScreen = ComponentManager::get_singleton_ptr()->add<Player::SplashScreenComponent>();
		XPLICIT_ASSERT(splashScreen);

		if (splashScreen)
			splashScreen->connect(dns);
	}

	Application::~Application() {}

	void Application::create_context()
	{
		auto singleton = Xplicit::Root::get_singleton_ptr();

		Xplicit::Root::get_singleton_ptr()->set(new Xplicit::InputReceiver());

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
		: mSettingsPath()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettingsPath = dat;
		mSettingsPath += "ClientSettings.dat";

		mIni = std::make_unique<mINI::INIFile>(mSettingsPath);
	}

	Application::SettingsManager::~SettingsManager() = default;

	void Application::SettingsManager::write(Application::SettingsManager::Traits& traits)
	{
		mINI::INIStructure struc;

		struc["Window"]["Width"] = std::to_string(traits.window_width);
		struc["Window"]["Height"] = std::to_string(traits.window_height);

		mIni->write(struc, true);
	}

	/* reads the clientsettings.dat INI file */
	void Application::SettingsManager::read(Application::SettingsManager::Traits& traits)
	{
		mINI::INIStructure struc;
		mIni->read(struc);

		try
		{
			auto width = struc["Window"]["Width"];
			auto widthInt = std::atoi(width.c_str());

			auto height = struc["Window"]["Height"];
			auto heightInt = std::atoi(width.c_str());

			traits.window_width = widthInt;
			traits.window_height = heightInt;

			if (traits.window_width > 1280 ||
				traits.window_height > 720)
			{
				traits.window_width = 1280;
				traits.window_height = 720;
			}
		}
		catch (...)
		{
			traits.window_height = 720;
			traits.window_width = 1280;
		}
	}
}
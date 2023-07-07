/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

// Application framework.
#include "Application.h"

//! We need this to connect and download from server. 
//! We also need the RoXML header.

#include "LoadingComponent.h"
#include "RoXML.h"

extern void XplicitLoadClientLua() noexcept;

namespace Xplicit::Player
{
	Vector<float> XPLICIT_DIM = Vector<float>(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	Application::Application(Utils::UriParser& xconnect_to)
		: mPath(""), mWsa()
	{
		//! Setup Engine
		this->setup();

		//! Load Basic lua calls
		XplicitLoadBaseLua();

		//! Register clientside Lua calls, such as PlaySound
		XplicitLoadClientLua();

		ComponentSystem::get_singleton_ptr()->add<NetworkComponent>();

		const auto splash_screen = ComponentSystem::get_singleton_ptr()->add<Player::LoadingComponent>();
		XPLICIT_ASSERT(splash_screen);

		splash_screen->connect(xconnect_to);
	}

	Application::~Application() {}

	void Application::setup()
	{
		SIrrlichtCreationParameters params;

		params.DriverMultithreaded = true;
		params.DriverType = EDT_OPENGL;
		params.Fullscreen = false;
		params.WindowSize = dimension2d<irr::u32>(Xplicit::Player::XPLICIT_DIM.X, Xplicit::Player::XPLICIT_DIM.Y);

		Root::get_singleton_ptr()->set(
			createDeviceEx(params)
		);

		RENDER->setWindowCaption(XPLICIT_APP_NAME);

		Root::get_singleton_ptr()->set(new InputReceiver());
		RENDER->setEventReceiver(Root::get_singleton_ptr()->Keyboard);

		Xplicit::init_winsock(&mWsa);

		//! Setup program contents path.
		XPLICIT_GET_DATA_DIR(path);
		mPath = path;

		//! Setup SettingsManager
		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);

		XPLICIT_GET_DATA_DIR(dir);

		String prebuilt = dir;
		prebuilt += "/Textures/DefaultSkin.zip";

		if (!RENDER->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true))
			throw EngineError("Missing Textures! This pack is needed for the XplicitPlayer to work.");
	}

	Application::SettingsManager::SettingsManager()
		: mSettingsPath()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettingsPath = dat;
		mSettingsPath += "ClientSettings.ini";

		mIni = std::make_unique<mINI::INIFile>(mSettingsPath);
	}

	Application::SettingsManager::~SettingsManager() = default;

	Application::SettingsManager& Application::SettingsManager::operator <<(Traits& traits)
	{
		mINI::INIStructure ini;

		ini["Window"]["Width"] = std::to_string(traits.window_width);
		ini["Window"]["Height"] = std::to_string(traits.window_height);

		mIni->write(ini, true);

		return *this;
	}

	/* reads the ClientSettings.dat INI file */
	Application::SettingsManager& Application::SettingsManager::operator >>(Traits& traits)
	{
		mINI::INIStructure ini;
		mIni->read(ini);

		try
		{
			const auto width_int = std::atoi(ini["Window"]["Width"].c_str());
			const auto height_int = std::atoi(ini["Window"]["Height"].c_str());

			traits.window_width = width_int;
			traits.window_height = height_int;

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

		return *this;
	}
}
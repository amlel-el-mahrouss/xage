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

// RoXML format
#include "RoXML.h"

// We need this to connect and download from server.
#include "LoadingComponent.h"

extern void XplicitLoadClientLua() noexcept;

namespace Xplicit::Player
{
	Vector<float> XPLICIT_DIM = Vector<float>(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	static void xplicit_open_skins()
	{
		XPLICIT_GET_DATA_DIR(dir);

		String prebuilt = dir;
		prebuilt += "\\Textures\\DefaultSkin.zip";

		if (!RENDER->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true))
			throw std::runtime_error("Missing skin! This pack is needed for the XplicitPlayer to work.");
	}

	Application::Application(Utils::UriParser& xconnect_to)
		: mPath(""), mWsa()
	{
		Xplicit::init_winsock(&mWsa);

		this->create_and_set_contexts();

		XPLICIT_GET_DATA_DIR(data_appdata);
		mPath += data_appdata;

		// register lua calls, such as PlaySound
		XplicitLoadClientLua();
		XplicitLoadBaseLua();

		const auto splash_screen = ComponentManager::get_singleton_ptr()->add<Player::LoadingComponent>();
		XPLICIT_ASSERT(splash_screen);

		// pass this
		splash_screen->set_data(mGwenManager->Canvas);
		
		splash_screen->connect(xconnect_to);
	}

	Application::~Application() {}

	void Application::create_and_set_contexts()
	{
		// Do not memset zero this!
		// It has values set by default.

		SIrrlichtCreationParameters params;

		params.DriverMultithreaded = true;
		params.DriverType = EDT_OPENGL;
		params.Fullscreen = false;
		params.WindowSize = dimension2d<irr::u32>(Xplicit::Player::XPLICIT_DIM.X, Xplicit::Player::XPLICIT_DIM.Y);

		Root::get_singleton_ptr()->set(
			createDeviceEx(params)
		);

		// Vector = Size of viewport. Only X and Y and taken into consideration.
		mGwenManager = std::make_unique<GWENComponentManager>(Vector<float>(Xplicit::Player::XPLICIT_DIM.X, 
			Xplicit::Player::XPLICIT_DIM.Y, 
			0.0f));

		Root::get_singleton_ptr()->set(new InputReceiver(mGwenManager->Canvas));

		xplicit_open_skins();

		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);

		RENDER->setEventReceiver(Root::get_singleton_ptr()->Keyboard);

		RENDER->setWindowCaption(Xplicit::Bites::XPLICIT_APP_NAME);
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
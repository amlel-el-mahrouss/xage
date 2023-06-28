/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"

// RoXML format
#include "RoXML.h"

// We need this to connect and download from server.
#include "LoadingComponent.h"

extern void xplicit_load_lua() noexcept;

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
		this->create_and_set_contexts();

		XPLICIT_GET_DATA_DIR(data_appdata);
		mPath += data_appdata;

		// register lua calls, such as PlaySound
		xplicit_load_lua();

		const auto splash_screen = ComponentManager::get_singleton_ptr()->add<Player::LoadingComponent>();
		XPLICIT_ASSERT(splash_screen);

		splash_screen->connect(xconnect_to);
	}

	Application::~Application() {}

	void Application::create_and_set_contexts()
	{
		Root::get_singleton_ptr()->set(
			createDevice(
				EDT_OPENGL,
				dimension2d<irr::u32>(Xplicit::Player::XPLICIT_DIM.X, Xplicit::Player::XPLICIT_DIM.Y),
				32U,
				false,
				false,
				false,
				nullptr
			)
		);

		mGwenManager = std::make_unique<GWENComponentManager>(Vector<float>(0.0f, 0.0f, 0.0f));
		Root::get_singleton_ptr()->set(new InputReceiver(mGwenManager->Canvas));

		Gwk::Controls::Button* button = new Gwk::Controls::Button(mGwenManager->Canvas);
		button->SetText("XplicitNgin now supports GUI!");
		button->SetPos(10, 10);

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
		mSettingsPath += "ClientSettings.dat";

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
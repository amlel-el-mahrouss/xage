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

// We need this to connect and download from server.
#include "LoadingComponent.h"
// RoXML format
#include "RoXML.h"

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
		prebuilt += "Textures/DefaultSkin.zip";

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dir, "FileSystem", XPLICIT_RES_GROUP);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(prebuilt, "Zip", XPLICIT_RES_GROUP);
	}

	Application::Application(Utils::UriParser& xconnect_to)
		: mPath(""), mWsa()
	{
		Xplicit::init_winsock(&mWsa);

		this->create_and_set_contexts();

		XPLICIT_GET_DATA_DIR(data_appdata);
		mPath += data_appdata;

		// Load Basic lua calls
		XplicitLoadBaseLua();

		// register lua calls, such as PlaySound
		XplicitLoadClientLua();

		const auto splash_screen = ComponentManager::get_singleton_ptr()->add<Player::LoadingComponent>();
		XPLICIT_ASSERT(splash_screen);

		splash_screen->connect(xconnect_to);

		Thread job([]() {
			RENDER->startRendering();

			while (true)
			{
				if (RENDER->getAutoCreatedWindow()->isClosed()) break;

				if (!RENDER->renderOneFrame()) break;
			}
		});

		job.detach();
	}

	Application::~Application() {}

	void Application::create_and_set_contexts()
	{
		Ogre::SceneManager* sceneManager = RENDER->createSceneManager();

		Root::get_singleton_ptr()->Ogre3D_Window->resize(Player::XPLICIT_DIM.X, Player::XPLICIT_DIM.Y);
		Root::get_singleton_ptr()->Ogre3D_Window->setVSyncEnabled(false);

		xplicit_open_skins();

		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);
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
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
#include "SplashScreenComponent.h"

extern void xplicit_load_lua();

namespace Xplicit::Player
{
	Vector<float> XPLICIT_DIM = Vector<float>(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	static void open_prebuilt_dialogs()
	{
		XPLICIT_GET_DATA_DIR(dir);

		String prebuilt = dir;
		prebuilt += "\\Textures\\DefaultSkin.zip";

		if (!RENDER->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true))
			throw std::runtime_error("Missing skin! This pack is needed for the XplicitPlayer to work.");
	}

	Application::Application(Utils::UriParser& xconnect_to)
		: mSettings(), mWsa(), mPath("")
	{
		this->create_context();

		XPLICIT_GET_DATA_DIR(tmp);
		mPath += tmp;

		XPLICIT_ASSERT(!mPath.empty());

#ifdef XPLICIT_WINDOWS
		Xplicit::init_winsock(&mWsa);
#endif

		const auto splash_screen = ComponentManager::get_singleton_ptr()->add<Player::SplashScreenComponent>();
		XPLICIT_ASSERT(splash_screen);

		splash_screen->connect(xconnect_to);
	}

	Application::~Application() {}

	void Application::create_context()
	{
		const auto singleton = Root::get_singleton_ptr();

		Root::get_singleton_ptr()->set(new InputReceiver());

		singleton->set(
			irr::createDevice(
				EDT_OPENGL,
				dimension2d<irr::u32>(Xplicit::Player::XPLICIT_DIM.X, Xplicit::Player::XPLICIT_DIM.Y),
				32U,
				false,
				false,
				false,
				KB
			)
		);

		open_prebuilt_dialogs();

		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);

		RENDER->setWindowCaption(Xplicit::Bites::XPLICIT_APP_NAME);

		// register lua calls, such as PlaySound
		xplicit_load_lua();
	}

	Application::SettingsManager::SettingsManager()
		:
	mSettingsPath()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettingsPath = dat;
		mSettingsPath += "ClientSettings.dat";

		mIni = std::make_unique<mINI::INIFile>(mSettingsPath);
	}

	Application::SettingsManager::~SettingsManager() = default;

	Application::SettingsManager& Application::SettingsManager::operator <<(Traits& traits)
	{
		mINI::INIStructure struc;

		struc["Window"]["Width"] = std::to_string(traits.window_width);
		struc["Window"]["Height"] = std::to_string(traits.window_height);

		mIni->write(struc, true);

		return *this;
	}

	/* reads the ClientSettings.dat INI file */
	Application::SettingsManager& Application::SettingsManager::operator >>(Traits& traits)
	{
		mINI::INIStructure struc;
		mIni->read(struc);

		try
		{
			const auto width_int = std::atoi(struc["Window"]["Width"].c_str());
			const auto height_int = std::atoi(struc["Window"]["Height"].c_str());

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
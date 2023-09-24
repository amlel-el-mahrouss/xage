/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "App.h"

#include "LoadingComponent.h"

#include <LuaAPI.h>

extern void XplicitLoadClientLua() noexcept;

namespace XPX
{
	Vector<float> XPLICIT_DIM = Vector<float>(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace XPX::Bites
{
	Application::Application(Utils::UriParser xconnect_to)
		: mPath("")
#ifdef XPLICIT_WINDOWS
		, mWsa()
#endif
	{
		this->setup_engine();

		ComponentSystem::get_singleton_ptr()->add<NetworkComponent>();

		XplicitLoadBaseLua();
		XplicitLoadClientLua();

		const auto loading_screen = ComponentSystem::get_singleton_ptr()->add<LoadingComponent>();
		XPLICIT_ASSERT(loading_screen);

		loading_screen->connect(xconnect_to);
	}

	Application::~Application() {}

	void Application::setup_engine()
	{
		//! Setup program contents path.
		XPLICIT_GET_DATA_DIR(path);
		mPath = path;

		//! Setup SettingsManager
		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);

		SettingsManager::Traits traits;

		(*mSettings) >> traits;

		if (traits.window_height < 600 ||
			traits.window_width < 800)
			throw EngineError("The Engine doesn't support high DPI displays.");

		SIrrlichtCreationParameters params;

		params.DriverMultithreaded = false;
		params.DriverType = EDT_DIRECT3D9;
		params.Fullscreen = false;
		params.WindowSize = dimension2d<irr::u32>(traits.window_width, traits.window_height);

		XPLICIT_DIM.X = traits.window_width;
		XPLICIT_DIM.Y = traits.window_height;

		Root::get_singleton_ptr()->set(
                createDeviceEx(params)
		);

		CAD->setWindowCaption(XPLICIT_APP_NAME);

		Root::get_singleton_ptr()->set(new InputReceiver());
		CAD->setEventReceiver(Root::get_singleton_ptr()->Keyboard);

#ifdef XPLICIT_WINDOWS
		XPX::init_winsock(&mWsa);
#endif

		//! Fetch our file first.
		String path_ini(path);
		path_ini += "ClientSettings.ini";

		//! Check for the existence of this ini file.
		if (!std::filesystem::exists(path_ini))
		{
			traits.window_width = XPLICIT_DIM.X;
			traits.window_height = XPLICIT_DIM.Y;
			traits.mouse_sensitivity = 1.0f;

			(*mSettings) << traits;
		}

		String prebuilt = mPath;
		prebuilt += "Textures/Default.zip";

		if (!CAD->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true))
			throw EngineError("Missing Textures! This pack is needed for the XPXPlayer to work.");
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
		try
		{
			mINI::INIStructure ini;
			mIni->read(ini);

			const auto width_int = std::atoi(ini["Window"]["Width"].c_str());
			const auto height_int = std::atoi(ini["Window"]["Height"].c_str());

			traits.window_width = width_int;
			traits.window_height = height_int;

			if (traits.window_width < 1280 ||
				traits.window_height < 720)
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
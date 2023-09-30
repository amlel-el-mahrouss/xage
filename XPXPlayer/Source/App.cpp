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
#include "LoadingScreenComponent.h"

namespace XPX::Bites
{
	ApplicationManager::ApplicationManager(Utils::UriParser xconnect_to)
		: mPath(""),
		mWsa(),
		mRenderer(nullptr),
		mRenderer2D(nullptr),
		mRenderingWindow(nullptr)
	{
		try
		{
			this->setup_engine();

			ComponentSystem::get_singleton_ptr()->add<NetworkComponent>();

			const auto loading_screen = ComponentSystem::get_singleton_ptr()->add<LoadingScreenComponent>();
			XPLICIT_ASSERT(loading_screen);

			loading_screen->connect(xconnect_to);
		}
		catch (EngineError& err)
		{
			std::wstring exit;
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

			exit += L"WHAT: ";
			exit += converter.from_bytes(err.what());
			exit += L"\n";

			XPX::DialogHelper::message_box(L"XAGE",
				L"Fatal error!",
				exit.c_str(),
				TD_INFORMATION_ICON,
				TDCBF_OK_BUTTON);
		}
	}

	ApplicationManager::~ApplicationManager() = default;

	void ApplicationManager::setup_engine()
	{
		//! Setup program contents path.
		XPLICIT_GET_DATA_DIR(path);
		mPath = path;

		//! Setup SettingsManager
		mSettings = new SettingsManager();
		XPLICIT_ASSERT(mSettings);

		SettingsManager::Traits traits;

		(*mSettings) >> traits;

		if (traits.window_height < 600 ||
			traits.window_width < 800)
			throw EngineError("XAGE doesn't support DPI displays.");

#ifdef XPLICIT_WINDOWS
		XPX::init_winsock(&mWsa);
#endif

		//! Fetch our file first.
		String path_ini(path);
		path_ini += "XAGE.cfg";

		//! Check for the existence of this ini file.
		if (!std::filesystem::exists(path_ini))
		{
			traits.window_width = XPLICIT_MIN_WIDTH;
			traits.window_height = XPLICIT_MIN_HEIGHT;
			traits.mouse_sensitivity = 1.0f;

			(*mSettings) << traits;
		}

		mRenderingWindow = new Win32Window("XPX Advanced Game Engine", "XAGEWindowClass", GetModuleHandle(nullptr));
		Root::get_singleton_ptr()->Window = mRenderingWindow;

		mRenderer = new Renderer::DX11::DriverSystemD3D11(mRenderingWindow->get().WindowHandle, traits.window_width, traits.window_height);
		Root::get_singleton_ptr()->Renderer = mRenderer;

		mRenderer2D = new Renderer::D2D::DriverSystemD2D(mRenderer);
		Root::get_singleton_ptr()->Renderer2D = mRenderer2D;

		Root::get_singleton_ptr()->Keyboard = new InputReceiver();
	}

	ApplicationManager::SettingsManager::SettingsManager()
		: mSettingsPath()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettingsPath = dat;
		mSettingsPath += "ClientSettings.ini";

		mIni = std::make_unique<mINI::INIFile>(mSettingsPath);
	}

	ApplicationManager::SettingsManager::~SettingsManager() = default;

	ApplicationManager::SettingsManager& ApplicationManager::SettingsManager::operator <<(Traits& traits)
	{
		mINI::INIStructure ini;

		ini["Window"]["Width"] = std::to_string(traits.window_width);
		ini["Window"]["Height"] = std::to_string(traits.window_height);

		mIni->write(ini, true);

		return *this;
	}

	/* reads the ClientSettings.dat INI file */
	ApplicationManager::SettingsManager& ApplicationManager::SettingsManager::operator >>(Traits& traits)
	{
		try
		{
			mINI::INIStructure ini;

			if (!mIni->read(ini))
				throw EngineError("File not found!");

			const auto width_int = std::atoi(ini["Window"]["Width"].c_str());
			const auto height_int = std::atoi(ini["Window"]["Height"].c_str());

			traits.window_width = width_int;
			traits.window_height = height_int;
		}
		catch (...)
		{
			XPLICIT_CRITICAL("NOT FOUND! Configuration file for XAGE wasn't found!");
		}

		if (traits.window_width < XPLICIT_DEFAULT_WIDTH ||
			traits.window_height < XPLICIT_DEFAULT_HEIGHT)
		{
			traits.window_width = XPLICIT_DEFAULT_WIDTH;
			traits.window_height = XPLICIT_DEFAULT_HEIGHT;
		}

		return *this;
	}
}
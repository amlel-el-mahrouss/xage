/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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

		std::string prebuilt = dir;
		prebuilt += "\\Textures\\PreBuiltDialogs.zip";

		auto res = IRR->getFileSystem()->addZipFileArchive(prebuilt.c_str(), true, true);

		XPLICIT_ASSERT(res);
		
		if (!res) 
			std::exit(3);
	}

	Application::Application(const char* dns)
		: mSettings(), mWsa(), mPath("")
	{
		XPLICIT_GET_DATA_DIR(tmp);
		mPath += tmp;

		XPLICIT_ASSERT(!mPath.empty());

#ifdef XPLICIT_WINDOWS
		Xplicit::init_winsock(&mWsa);
#endif

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		this->setup();

		auto loadingComponent = ComponentManager::get_singleton_ptr()->add<Player::LoadingComponent>();
		XPLICIT_ASSERT(loadingComponent);

		if (loadingComponent)
			loadingComponent->connect(dns);
	}

	Application::~Application() {}

	void Application::setup()
	{
		Xplicit::ApplicationContext::get_singleton_ptr()->set(new Xplicit::InputReceiver());

		if (!KB)
			throw EngineError();

		Xplicit::ApplicationContext::get_singleton_ptr()->set(
			irr::createDevice(
				irr::video::EDT_OPENGL,
				Xplicit::Player::XPLICIT_DIM,
				32U,
				false,
				false,
				false,
				KB
			)
		);

		open_prebuilt_dialogs();

		mSettings = std::make_unique<Settings>();

		XPLICIT_ASSERT(mSettings);

		if (!mSettings)
			throw EngineError();

		Settings::Traits traits{};
		mSettings->read(traits);

		IRR->setWindowCaption(Xplicit::Bites::XPLICIT_APP_NAME);
	}

	Application::Settings::Settings()
		: mSettings()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettings = dat;
		mSettings += "ClientSettings.xml";
	}

	Application::Settings::~Settings() = default;

	void Application::Settings::write(Application::Settings::Traits& traits)
	{
		
	}

	void Application::Settings::read(Application::Settings::Traits& traits)
	{
		
	}
}
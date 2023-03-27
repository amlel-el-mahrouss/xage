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

namespace Xplicit::Bites
{
	Application::Application(const char* dns, HINSTANCE hInstance)
		: m_pSettings(), m_wsa(), 
		m_szDataPath(""), m_win32Window(XPLICIT_APP_NAME, XPLICIT_CLASS_NAME, hInstance)
	{
		XPLICIT_GET_DATA_DIR(data_tmp);
		m_szDataPath += data_tmp;

		XPLICIT_ASSERT(!m_szDataPath.empty());

#ifdef XPLICIT_WINDOWS
		Xplicit::init_winsock(&m_wsa);
#endif

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		this->setup();

		auto loadingComponent = ComponentManager::get_singleton_ptr()->add<Client::LoadingComponent>();
		XPLICIT_ASSERT(loadingComponent);

		if (loadingComponent)
			loadingComponent->connect(dns);
	}

	Application::~Application() {}

	void Application::setup()
	{
		HMENU hMenu = GetSystemMenu(m_win32Window.get().WindowHandle, false);
		EnableMenuItem(hMenu, SC_CLOSE, MF_GRAYED);

		m_pSettings = std::make_unique<Settings>();

		XPLICIT_ASSERT(m_pSettings);

		if (!m_pSettings)
			throw EngineError();

		Settings::Traits traits{};
		m_pSettings->read(traits);

	}

	Application::Settings::Settings()
		: m_szSettingsPath()
	{
		XPLICIT_GET_DATA_DIR(dat);

		m_szSettingsPath = dat;
		m_szSettingsPath += "ClientSettings.xml";

		XPLICIT_ASSERT(!m_szSettingsPath.empty());

	}

	Application::Settings::~Settings()
	{

	}

	void Application::Settings::write(Application::Settings::Traits& traits)
	{
		
	}

	void Application::Settings::read(Application::Settings::Traits& traits)
	{
		
	}
}
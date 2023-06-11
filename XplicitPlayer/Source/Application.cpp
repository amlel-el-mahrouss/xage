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

namespace Xplicit::Client
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
		if (!res) std::exit(3);
	}

	Application::Application(const char* dns)
		: m_pSettings(), m_wsa(), m_szDataPath("")
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
		Xplicit::ApplicationContext::get_singleton_ptr()->set(new Xplicit::InputReceiver());

		if (!KB)
			throw EngineError();

		Xplicit::ApplicationContext::get_singleton_ptr()->set(
			irr::createDevice(
				irr::video::EDT_OPENGL,
				Xplicit::Client::XPLICIT_DIM,
				32U,
				false,
				false,
				false,
				KB
			)
		);

		open_prebuilt_dialogs();

		m_pSettings = std::make_unique<Settings>();

		XPLICIT_ASSERT(m_pSettings);

		if (!m_pSettings)
			throw EngineError();

		Settings::Traits traits{};
		m_pSettings->read(traits);

		IRR->setWindowCaption(Xplicit::Bites::XPLICIT_APP_NAME);
	}

	Application::Settings::Settings()
		: m_szSettingsPath(), m_pXmlReader(nullptr), m_pXmlWriter(nullptr)
	{
		XPLICIT_GET_DATA_DIR(dat);

		m_szSettingsPath = dat;
		m_szSettingsPath += "ClientSettings.xml";

		XPLICIT_ASSERT(!m_szSettingsPath.empty());

		m_pXmlReader = IRR->getFileSystem()->createXMLReaderUTF8(m_szSettingsPath.c_str());
		m_pXmlWriter = IRR->getFileSystem()->createXMLWriter(m_szSettingsPath.c_str());
	}

	Application::Settings::~Settings()
	{
		if (m_pXmlWriter)
			m_pXmlWriter->drop();

		if (m_pXmlReader)
			m_pXmlReader->drop();
	}

	void Application::Settings::write(Application::Settings::Traits& traits)
	{
		if (m_pXmlWriter)
		{
			m_pXmlWriter->writeComment(L"Generated by XplicitPlayer (www.play-xplicit.com)");

			m_pXmlWriter->writeLineBreak();

			m_pXmlWriter->writeElement(L"Window");

			m_pXmlWriter->writeElement(L"Dim",
				true,
				L"Width",
				std::to_wstring(traits.window_width).c_str(),
				L"Height",
				std::to_wstring(traits.window_height).c_str());

			m_pXmlWriter->writeClosingTag(L"Window");
		}
	}

	void Application::Settings::read(Application::Settings::Traits& traits)
	{
		if (m_pXmlReader)
		{
			while (m_pXmlReader->read())
			{
				switch (m_pXmlReader->getNodeType())
				{
				case irr::io::EXN_ELEMENT:
				{

					break;
				}
				}
			}
		}
	}
}
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
		: m_settings(), m_wsa(), m_data_path("")
	{
		XPLICIT_GET_DATA_DIR(data_tmp);
		m_data_path += data_tmp;

		XPLICIT_ASSERT(!m_data_path.empty());

#ifdef XPLICIT_WINDOWS
		Xplicit::init_winsock(&m_wsa);
#endif

#ifdef XPLICIT_DEBUG
		Xplicit::open_terminal();
#endif

		this->setup();

		auto loader = ComponentManager::get_singleton_ptr()->add<Client::LoadingComponent>();
		XPLICIT_ASSERT(loader);

		if (loader)
			loader->connect(dns);
	}

	Application::~Application()
	{

	}

	void Application::setup()
	{
		Xplicit::ApplicationContext::get_singleton_ptr()->set(new Xplicit::InputReceiver());

		if (!KB) std::exit(3);

		Xplicit::ApplicationContext::get_singleton_ptr()->set(irr::createDevice(irr::video::EDT_DIRECT3D9,
			Xplicit::Client::XPLICIT_DIM,
			32U,
			false,
			false,
			false,
			KB));

#ifdef XPLICIT_WINDOWS
		HMENU menuHandle = GetSystemMenu((HWND)IRR->getVideoDriver()->getExposedVideoData().D3D9.HWnd, false);
		EnableMenuItem(menuHandle, SC_CLOSE, MF_GRAYED);
#endif

		open_prebuilt_dialogs();

		m_settings = std::make_unique<Settings>();

		XPLICIT_ASSERT(m_settings);
		if (!m_settings) std::exit(3);

		Settings::Traits traits{};
		m_settings->read(traits);

		if (traits.window_width >= XPLICIT_DEFAULT_WIDTH && 
			traits.window_width >= XPLICIT_DEFAULT_HEIGHT)
			IRR->setWindowSize(dimension2d<irr::u32>(traits.window_width, traits.window_height));

		IRR->setWindowCaption(Xplicit::Bites::XPLICIT_APP_NAME);
	}

	// Application Settings

	static const char* XPLICIT_DEFAULT_SETTINGS = "<!-- XplicitNgin Settings -->\r\n<Window>\r\n<Dim Width = \"800\" Height = \"600\" />\r\n<MouseSpeed Value=\"1.0\"/>\r\n</Window>";

	Application::Settings::Settings()
		: m_settings_path(), m_xml_reader(nullptr), m_xml_writer(nullptr)
	{
		XPLICIT_GET_DATA_DIR(dat);

		m_settings_path = dat;
		m_settings_path += "\\ClientSettings.xml";

		XPLICIT_ASSERT(!m_settings_path.empty());

		m_xml_reader = IRR->getFileSystem()->createXMLReaderUTF8(m_settings_path.c_str());
		m_xml_writer = IRR->getFileSystem()->createXMLWriterUTF8(m_settings_path.c_str());
	}

	Application::Settings::~Settings()
	{
		if (m_xml_writer)
			m_xml_writer->drop();

		if (m_xml_reader)
			m_xml_reader->drop();
	}

	void Application::Settings::write(Application::Settings::Traits& traits)
	{
		if (m_xml_writer)
		{
			m_xml_writer->writeComment("Generated by XplicitPlayer (www.xplicit.com)");
			m_xml_writer->writeLineBreak();

			m_xml_writer->writeElement("Window");

			m_xml_writer->writeElement("Dim", true, "Width", 
				std::to_string(traits.window_width).c_str(), "Height",
				std::to_string(traits.window_height).c_str());

			m_xml_writer->writeClosingTag("Window");
		}
	}

	void Application::Settings::read(Application::Settings::Traits& traits)
	{
		if (m_xml_reader)
		{
			while (m_xml_reader->read())
			{
				switch (m_xml_reader->getNodeType())
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
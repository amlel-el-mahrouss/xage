/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: App.h
 *			Purpose: Application Framework
 *
 * =====================================================================
 */

#pragma once

#include <Event.h>
#include <Instance.h>
#include <Foundation.h>
#include <ApplicationContext.h>

#include "Camera.h"
#include "LocalActor.h"
#include "DriverD3D11.h"
#include "LoadingInstance.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

namespace Xplicit::Client
{
	static inline dimension2du XPLICIT_DIM = dimension2du(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	constexpr const wchar_t* XPLICIT_APP_NAME = L"XplicitNgin";

	class Application final
	{
	public:
		Application() = delete;

	public:
		Application(const char* ip);
		~Application();

		Application& operator=(const Application&) = delete;
		Application(const Application&) = delete;

		class Settings final
		{
		public:
			struct Traits
			{
				int window_width{ XPLICIT_DEFAULT_WIDTH };
				int window_height{ XPLICIT_DEFAULT_HEIGHT };
				float mouse_sensitivity{ 1.0f };
			};

		public:
			Settings();
			~Settings();

			void write(Traits& traits);
			void read(Traits& traits);

		private:
			std::string m_settings_path;

		private:
			irr::io::IXMLReaderUTF8* m_xml_reader;
			irr::io::IXMLWriterUTF8* m_xml_writer;

		};

	private:
		void setup();

	private:
		std::unique_ptr<Settings> m_settings;
		std::string m_data_path;
		WSADATA m_wsa;

	};
}
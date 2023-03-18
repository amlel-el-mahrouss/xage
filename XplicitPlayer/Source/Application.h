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

// URLs
#include <Uri.h>

#include "Camera.h"
#include "LocalActor.h"
#include "DriverD3D11.h"
#include "LoadingComponent.h"

// Xplicit Bites library
#include <Bites.h>

// Application Context (will be deprecated soon)
#include <ApplicationContext.h>

// will be deprecated soon.
namespace Xplicit::Client
{
	static inline dimension2du XPLICIT_DIM = dimension2du(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	constexpr const pchar* XPLICIT_APP_NAME = L"XplicitNgin";

	class Application final
	{
	public:
		Application() = delete;

	public:
		Application(const char* dns);
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

#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA m_wsa;
#endif

	};
}
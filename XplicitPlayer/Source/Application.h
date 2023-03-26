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

#include "LocalPlayerComponent.h"
#include "LoadingComponent.h"
#include "CameraComponent.h"

// Application Context (will be deprecated soon)
#include <ApplicationContext.h>

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

// will be deprecated soon.
namespace Xplicit::Client
{
	static inline dimension2du XPLICIT_DIM = dimension2du(XPLICIT_DEFAULT_WIDTH, XPLICIT_DEFAULT_HEIGHT);
}

namespace Xplicit::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"Xplicit Engine";

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
			String m_szSettingsPath;
			irr::io::IXMLReaderUTF8* m_pXmlReader;
			irr::io::IXMLWriterUTF8* m_pXmlWriter;

		};

	private:
		void setup();

	private:
		std::unique_ptr<Settings> m_pSettings;
		String m_szDataPath;

#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA m_wsa;
#endif

	};
}
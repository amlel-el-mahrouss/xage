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

#include <Uri.h>
#include <ApplicationContext.h>

#include "CameraComponent.h"
#include "LoadingComponent.h"
#include "LocalPlayerComponent.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

namespace Xplicit::Client
{
	extern dimension2du XPLICIT_DIM;
}

namespace Xplicit::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"XplicitNgine - www.play-xplicit.com";

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
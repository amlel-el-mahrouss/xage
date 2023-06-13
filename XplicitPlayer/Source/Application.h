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

/* engine classes */
#include <Uri.h>
#include <IXMLWriter.h>
#include <ApplicationContext.h>

/* client classes */
#include "CameraComponent.h"
#include "LoadingComponent.h"
#include "LocalPlayerComponent.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

namespace Xplicit::Player
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
			String mSettings;

		};

	private:
		void setup();

	private:
		std::unique_ptr<Settings> mSettings;
		String mPath;

#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA mWsa;
#endif

	};
}
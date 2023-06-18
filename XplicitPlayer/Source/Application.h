/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: App.h
 *			Purpose: Application Framework
 *
 * =====================================================================
 */

#pragma once

#include <IXMLWriter.h>
#include <Root.h>
#include <Uri.h>
#include <ini.h>

/* client classes */
#include "LocalCameraComponent.h"
#include "SplashScreenComponent.h"
#include "LocalPlayerComponent.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

namespace Xplicit::Player
{
	extern dimension2du XPLICIT_DIM;
}

namespace Xplicit::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"Xplicit - www.play-xplicit.com";

	class Application final
	{
	public:
		Application() = delete;

	public:
		Application(const char* xconnectTo);
		~Application();

	public:
		class SettingsManager final
		{
		public:
			struct Traits
			{
				int window_width{ XPLICIT_DEFAULT_WIDTH };
				int window_height{ XPLICIT_DEFAULT_HEIGHT };
				float mouse_sensitivity{ 1.0f };
			};

		public:
			SettingsManager();
			~SettingsManager();

			void write(Traits& traits);
			void read(Traits& traits);

		private:
			std::unique_ptr<mINI::INIFile> mIni;
			String mSettingsPath;

		};

	private:
		void create_context();

	private:
		std::unique_ptr<SettingsManager> mSettings;
		String mPath;

#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA mWsa;
#endif

	};
}
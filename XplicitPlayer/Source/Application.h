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
#include <Bites.h>
#include <bgfx/bgfx.h>
#include <HelperMacros.h>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include "CameraComponent.h"
#include "LoadingComponent.h"
#include "LocalPlayerComponent.h"

#define XPLICIT_DEFAULT_WIDTH (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

namespace Xplicit::Bites
{
	constexpr const char* XPLICIT_APP_NAME = "XplicitNgine - www.play-xplicit.com";
	constexpr const char* XPLICIT_CLASS_NAME = "XplicitWnd";

	class Application final
	{
	public:
		Application() = delete;

	public:
		Application(const char* dns, HINSTANCE hInst);
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

		};

	private:
		void setup();

	private:
		std::unique_ptr<Settings> m_pSettings;
		wi::Application m_applicationWindow;
		Win32Window m_win32Window;
		String m_szDataPath;
		WSADATA m_wsa;

	};
}
/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <IXMLWriter.h>
#include <Nplicit.h>
#include <Root.h>
#include <Uri.h>
#include <ini.h>

#define XPLICIT_DEFAULT_WIDTH  (1280)
#define XPLICIT_DEFAULT_HEIGHT (720)

namespace Xplicit::Player
{
	extern Vector<float> XPLICIT_DIM;
} // namespace Xplicit::Player

namespace Xplicit::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"XplicitNgine - www.play-xplicit.com";

	class Application final
	{
	public:
		Application() = delete;

	public:
		Application(Utils::UriParser& xconnectTo);
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

			SettingsManager& operator <<(Traits& traits);
			SettingsManager& operator >>(Traits& traits);

		private:
			std::unique_ptr<mINI::INIFile> mIni;
			String mSettingsPath;

		};

	private:
		void create_context();
		
		std::unique_ptr<SettingsManager> mSettings;
		String mPath;

#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA mWsa;
#endif

	};
} // namespace Xplicit::Player
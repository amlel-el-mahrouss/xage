/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "PlayerConfig.h"

#include <lua/lua.hpp>
#include <Nplicit.h>
#include <Bites.h>
#include <Root.h>
#include <Uri.h>
#include <ini.h>

namespace Xplicit
{
	namespace Player
	{
		extern Vector<float> XPLICIT_DIM;
	}
} // namespace Xplicit::Player

namespace Xplicit::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"Xplicit - [ Loading... ]";

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

	public:
		SettingsManager* leak_settings() const noexcept { return mSettings.get(); }

	private:
		void create_and_set_contexts();
		
	private:
		std::unique_ptr<SettingsManager> mSettings;

	private:
		String mPath;


	private:
#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA mWsa;
#endif

	};
} // namespace Xplicit::Player
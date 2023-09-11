/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "PlayerConfig.h"

#include <CLua.hpp>
#include <Nplicit.h>
#include <Bites.h>
#include <Root.h>
#include <Uri.h>
#include <ini.h>

namespace XPX
{
	extern Vector<float> XPLICIT_DIM;
} // namespace Xplicit

namespace XPX::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"XPLICIT";

	class ObjectInputSystem final
	{
	public:
		explicit ObjectInputSystem() = default;
		~ObjectInputSystem() = default;

	public:
		XPLICIT_COPY_DEFAULT(ObjectInputSystem);

	public:
		static ObjectInputSystem* get_singleton_ptr() noexcept
		{
			ObjectInputSystem* singleton = nullptr;

			if (!singleton)
				singleton = new ObjectInputSystem();

			return singleton;
		}

	public:
		struct InputTraits
		{
			char mForward{ KEY_KEY_W };
			char mLeft{ KEY_KEY_A };
			char mRight{ KEY_KEY_D };
			char mBackward{ KEY_KEY_S };
			char mJump{ KEY_SPACE };

		};

	public:
		InputTraits Layout;

	};

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
		void setup();
		
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
} // namespace Xplicit
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

#include <lua/CLua.hpp>
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
	constexpr const char* XPLICIT_APP_NAME = "Xplicit - [ Place1 ]";

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

		bool key_down(const char key) noexcept
		{
			XPLICIT_ASSERT(Root::get_singleton_ptr());
			return Root::get_singleton_ptr()->KeyboardEvent.keysym.sym == key;
		}

	public:
		struct InputTraits
		{
			int16_t mForward{ L'W' };
			int16_t mLeft{ L'A' };
			int16_t mRight{ L'D' };
			int16_t mBackward{ L'S' };
			int16_t mJump{ VK_SPACE };

		};

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
} // namespace Xplicit::Player
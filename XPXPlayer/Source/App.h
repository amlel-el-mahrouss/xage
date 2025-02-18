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
#include <NplicitNgine.h>
#include <Bites.h>
#include <Root.h>
#include <Uri.h>
#include <ini.h>

namespace XPX::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"XPX Advanced Game Engine";

	class InputSystem final
	{
	public:
		explicit InputSystem() = default;
		~InputSystem() = default;

	public:
		XPLICIT_COPY_DEFAULT(InputSystem);

	public:
		static InputSystem* get_singleton_ptr() noexcept
		{
			InputSystem* singleton = nullptr;

			if (!singleton)
				singleton = new InputSystem();

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

	class ApplicationManager final
	{
	public:
		ApplicationManager() = delete;

	public:
		ApplicationManager(Utils::UriParser xconnectTo);
		~ApplicationManager();

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

		public:
			XPLICIT_COPY_DEFAULT(SettingsManager);

		public:
			SettingsManager& operator <<(Traits& traits);
			SettingsManager& operator >>(Traits& traits);

		private:
			std::unique_ptr<mINI::INIFile> mIni;
			String mSettingsPath;

		};

	public:
		SettingsManager* leak_settings() const noexcept { return mSettings; }

	private:
		void setup_engine();
		
	private:
		Renderer::DX11::DriverSystemD3D11* mRenderer;
		Renderer::D2D::DriverSystemD2D* mRenderer2D;
		Bites::Win32Window* mRenderingWindow;
		SettingsManager* mSettings;

	private:
		String mPath;

	private:
#ifdef XPLICIT_WINDOWS
		// WinSock data
		WSADATA mWsa;
#endif

	};
} // namespace Xplicit
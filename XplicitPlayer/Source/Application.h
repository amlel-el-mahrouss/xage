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

#define KB Xplicit::Root::get_singleton_ptr()->Keyboard

namespace Xplicit
{
	using namespace irr;
	using namespace core;
	using namespace gui;
	using namespace video;
	using namespace scene;

	class InputReceiver final : public irr::IEventReceiver
	{
	public:
		explicit InputReceiver()
			:
			mMouseLeft(),
			mMouseRight(),
			mMousePos(),
			mWheelEnable(false),
			mWheel(0.0f)
		{
			for (irr::u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
				mKeys[i] = 0;
		}

		bool OnEvent(const SEvent& env) override
		{
			mWheelEnable = false;

			if (env.EventType == EET_KEY_INPUT_EVENT)
				mKeys[env.KeyInput.Key] = env.KeyInput.PressedDown;

			if (env.EventType == EET_MOUSE_INPUT_EVENT)
			{
				switch (env.MouseInput.Event)
				{
				case EMIE_LMOUSE_PRESSED_DOWN:
				{
#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("World:LeftClick [EVENT]");
#endif // XPLICIT_DEBUG

					Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:LeftClick()");

					mMouseLeft.Down = true;
					break;
				}

				case EMIE_LMOUSE_LEFT_UP:
					mMouseLeft.Down = false;
					break;

				case EMIE_RMOUSE_PRESSED_DOWN:
				{
#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("World:RightClick [EVENT]");
#endif // XPLICIT_DEBUG

					Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:RightClick()");

					mMouseRight.Down = true;
					break;
				}

				case EMIE_MOUSE_WHEEL:
				{
					mWheelEnable = true;
					mWheel = env.MouseInput.Wheel;

					break;
				}

				case EMIE_RMOUSE_LEFT_UP:
					mMouseRight.Down = false;
					break;

				case EMIE_MOUSE_MOVED:
				{
					mMousePos.X = env.MouseInput.X;
					mMousePos.Y = env.MouseInput.Y;

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("World:MouseMove [EVENT]");
#endif // XPLICIT_DEBUG

					Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:MouseMove()");

					break;
				}

				default:
					break;
				}
			}

			return true;
		}

	private:
		struct MouseEventTraits
		{
			int32_t X;
			int32_t Y;

			bool Down;
		};

	public:
		virtual ~InputReceiver() = default;

	public:
		InputReceiver& operator=(const InputReceiver&) = default;
		InputReceiver(const InputReceiver&) = default;

	public:
		bool right_down() noexcept { return mMouseRight.Down; }
		bool left_down() noexcept { return mMouseLeft.Down; }

	public:
		float mouse_wheel() noexcept
		{
			if (mWheelEnable)
				return mWheel;

			return 0.f;
		}

		bool key_down(const char key) const
		{
			return mKeys[key];
		}

		bool key_down() const
		{
			for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
			{
				if (mKeys[i])
					return true;
			}

			return false;
		}

		MouseEventTraits& get_pos() noexcept { return mMousePos; }

	public:
		struct MovementTraits final
		{
		public:
			MovementTraits() = default;
			~MovementTraits() = default;

			char mBackward{ -1 };
			char mForward{ -1 };
			char mRight{ -1 };
			char mLeft{ -1 };
		};

	private:
		bool mKeys[KEY_KEY_CODES_COUNT];

	private:
		MouseEventTraits mMouseRight;
		MouseEventTraits mMouseLeft;
		MouseEventTraits mMousePos;

	private:
		bool mWheelEnable;
		float mWheel;

	};

	namespace Player
	{
		extern Vector<float> XPLICIT_DIM;
	}
} // namespace Xplicit::Player

namespace Xplicit::Bites
{
	constexpr const PChar* XPLICIT_APP_NAME = L"Xplicit - [ Place1 ]";

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
			return KB->key_down(key);
		}

		bool key_down() noexcept
		{
			return KB->key_down();
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
} // namespace Xplicit::Player
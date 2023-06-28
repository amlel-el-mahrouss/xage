/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "PlayerConfig.h"
#include "GWEN.h"

#include <lua/lua.hpp>
#include <Nplicit.h>
#include <Bites.h>
#include <Root.h>
#include <Uri.h>
#include <ini.h>

#define KB Xplicit::Root::get_singleton_ptr()->Keyboard

namespace Xplicit
{
	class InputReceiver final : public IEventReceiver
	{
	public:
		explicit InputReceiver(Gwk::IInputEventListener* el)
			: 
			mGwenInput(),
			mMouseLeft(),
			mMouseRight(),
			mMousePos(),
			mLayout()
		{
			mGwenInput.Initialize(el);

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
					XPLICIT_INFO("LocalHumanoid:Click [EVENT]");
					Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Click()");

					mMouseLeft.Down = true;
					break;
				}

				case EMIE_LMOUSE_LEFT_UP:
					mMouseLeft.Down = false;
					break;

				case EMIE_RMOUSE_PRESSED_DOWN:
				{
					XPLICIT_INFO("LocalHumanoid:RightClick [EVENT]");
					Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:RightClick()");

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

					XPLICIT_INFO("LocalHumanoid:MouseMove [EVENT]");
					Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:MouseMove()");

					break;
				}

				default:
					break;
				}
			}

			return mGwenInput.HandleEvents(env);
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

		MovementTraits& get_layout() noexcept
		{
			CHAR layout[KL_NAMELENGTH];
			GetKeyboardLayoutNameA(layout);

			String frFr = "0000040C"; // fr-FR

			auto str = String(layout);

			if (str == frFr)
			{
				mLayout.mForward = KEY_KEY_Z;
				mLayout.mRight = KEY_KEY_D;
				mLayout.mLeft = KEY_KEY_Q;
				mLayout.mBackward = KEY_KEY_S;
			}
			else
			{
				mLayout.mForward = KEY_KEY_W;
				mLayout.mRight = KEY_KEY_D;
				mLayout.mLeft = KEY_KEY_A;
				mLayout.mBackward = KEY_KEY_S;
			}

			return mLayout;
		}

	private:
		MovementTraits mLayout;
		bool mKeys[KEY_KEY_CODES_COUNT];

	private:
		Gwk::Input::Irrlicht mGwenInput;
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
	constexpr const PChar* XPLICIT_APP_NAME = L"Xplicit - [ No Place ]";

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
		GWENComponentManager* leak_gwen() const noexcept { return mGwenManager.get(); }
		SettingsManager* leak_settings() const noexcept { return mSettings.get(); }

	private:
		void create_and_set_contexts();
		
	private:
		std::unique_ptr<GWENComponentManager> mGwenManager;
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
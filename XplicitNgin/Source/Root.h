/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "HelperMacros.h"

#include <CLua/CLua.hpp>

//! The CAD Engine include.
#include <cad-engine/irrlicht.h>
#include <cad-engine/IEventReceiver.h>

 /* common engine macros for Root */
#define RENDER Xplicit::Root::get_singleton_ptr()->Dev
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

		char key_down() const
		{
			for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
			{
				if (mKeys[i])
					return i;
			}

			return 0;
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

	class XPLICIT_API Root final
	{
	private:
		explicit Root()
			: Keyboard(nullptr), ShouldExit(false), Dev(nullptr)
		{}

		~Root() noexcept
		{
			if (Dev)
				Dev->drop();

			ShouldExit = true;
		}

		Root& operator=(const Root&) = default;
		Root(const Root&) = default;

	public:
		static Root* get_singleton_ptr() noexcept
		{
			static Root* root = nullptr;

			if (!root)
				root = new Root();

			return root;
		}

	public:
		void set(irr::IrrlichtDevice* dev) noexcept
		{
			if (dev)
				Dev = dev;
		}

		void set(InputReceiver* kb) noexcept
		{
			if (kb)
				Keyboard = kb;
		}

	public:
		irr::IrrlichtDevice* Dev;
		InputReceiver* Keyboard;
		bool ShouldExit;

	};
}

/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: ApplicationContext.h
 *			Purpose: Application Context and helpers.
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

namespace Xplicit
{
	class XPLICIT_API InputReceiver final : public irr::IEventReceiver
	{
	public:
		virtual bool OnEvent(const SEvent& event)
		{
			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
				m_keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

			if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			{
				switch (event.MouseInput.Event)
				{
				case EMIE_LMOUSE_PRESSED_DOWN:
					m_mouse_left.Down = true;
					break;

				case EMIE_LMOUSE_LEFT_UP:
					m_mouse_left.Down = false;
					break;

				case EMIE_RMOUSE_PRESSED_DOWN:
					m_mouse_right.Down = true;
					break;

				case EMIE_RMOUSE_LEFT_UP:
					m_mouse_right.Down = false;
					break;

				case EMIE_MOUSE_MOVED:
					m_mouse_pos.X = event.MouseInput.X;
					m_mouse_pos.Y = event.MouseInput.Y;
					break;

				default:
					// We won't use the wheel
					break;
				}
			}

			return false;
		}

	private:
		struct MouseEventTraits
		{
			int32_t X;
			int32_t Y;
			bool Down;
		};

	public:
		InputReceiver()
			: m_mouse_left(), m_mouse_right(), m_mouse_pos()
		{
			for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
				m_keys[i] = false;
		}

		virtual ~InputReceiver() {}

		InputReceiver& operator=(const InputReceiver&) = default;
		InputReceiver(const InputReceiver&) = default;

		bool right_down() noexcept { return m_mouse_right.Down; }
		bool left_down() noexcept { return m_mouse_left.Down; }

		bool key_down(const EKEY_CODE& ky) const
		{
			return m_keys[ky];
		}

		bool key_down() const
		{
			for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
			{
				if (m_keys[i])
					return true;
			}

			return false;
		}

		MouseEventTraits& get_pos() noexcept { return m_mouse_pos; }

	private:
		bool m_keys[KEY_KEY_CODES_COUNT];

		MouseEventTraits m_mouse_right;
		MouseEventTraits m_mouse_left;
		MouseEventTraits m_mouse_pos;

	};

	class XPLICIT_API ApplicationContext final
	{
	private:
		ApplicationContext()
			: Keyboard(nullptr), Dev(nullptr), Reader(nullptr), Writer(nullptr), ShouldExit(false)
		{}

		~ApplicationContext() 
		{
			if (Dev)
				Dev->drop();

			if (Reader)
				Reader->drop();

			if (Writer)
				Writer->drop();
		}

		ApplicationContext& operator=(const ApplicationContext&) = default;
		ApplicationContext(const ApplicationContext&) = default;

	public:
		static ApplicationContext& get_singleton() noexcept
		{
			static ApplicationContext app;

			// return a static singleton, simple!
			return app;
		}

		void set(irr::IrrlichtDevice* dev)
		{
			if (dev)
				Dev = dev;
		}

		void set(InputReceiver* kb)
		{
			if (kb)
				Keyboard = kb;
		}

	public:
		irr::io::IIrrXMLReader<char, IReferenceCounted>* Reader;
		irr::io::IXMLWriter* Writer;
		irr::IrrlichtDevice* Dev;
		InputReceiver* Keyboard;
		bool ShouldExit;

	};

	class UUIDFactory final
	{
	public:
		template <int ver>
		static uuids::uuid version()
		{
			switch (ver)
			{
			case 4:
			{
				std::random_device rd;
				auto seed_data = std::array<int, std::mt19937::state_size> {};
				std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
				std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
				std::mt19937 generator(seq);

				auto gen = uuids::uuid_random_generator(generator);

				return gen();
			}
			}

			return {};
		}

	};
}

#define XML Xplicit::ApplicationContext::get_singleton().Reader
#define IRR  Xplicit::ApplicationContext::get_singleton().Dev
#define KB  Xplicit::ApplicationContext::get_singleton().Keyboard

#ifdef XPLICIT_WINDOWS
#ifndef XPLICIT_GET_DATA_DIR
#define XPLICIT_GET_DATA_DIR(DIR)\
char DIR[15];\
memcpy(DIR, "..\\Data\\", 15);

#endif
#else
#error You need XPLICIT_GET_DATA_DIR!
#endif

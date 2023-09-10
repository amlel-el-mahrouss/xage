/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

//! Xplicit ECS API

#include <Component.h>
#include <codecvt>
#include <Util.h>

//! Xplicit Sound API

#include <Xplicit.h>
#include <XplicitSound.h>
#include <ClassComponent.h>

namespace XPX
{
	class SoundComponent final : public ClassComponent
	{
	public:
		explicit SoundComponent(const char* name, const char* parent);
		~SoundComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(SoundComponent);

	public:
		void play(const String& path) noexcept;
		void play_2d(const String& path) noexcept;

	public:
		static void update(void* class_ptr);
		static bool should_update() noexcept;

		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;

	public:
		void set_volume(const float volume = 0.5f) noexcept;
		void set_position(const Vector<float> pos = Vector<float>(0.0f, 0.0f, 0.0f)) noexcept;
		void should_loop(const bool enable = true) noexcept;

	private:
		float mVolume{ 0.5f };
		float mPitch{ 100.f };
		float mPan{ 0 };
		bool mLoop{ false };

	};
}
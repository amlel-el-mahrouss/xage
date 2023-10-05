/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Component.h>
#include <codecvt>
#include <Util.h>

#include <NginCore.h>
#include <SoundDriver.h>

namespace XPX
{
	class SoundComponent final : public Component, public XPXAttribute
	{
	public:
		SoundComponent(const char* name, const char* parent);
		~SoundComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(SoundComponent);

	public:
		void play(const String& path) noexcept;

	public:
		static void update(void* class_ptr);
		static bool should_update() noexcept;

		const char* instance_name() noexcept { return mName.c_str(); }
		const char* group_name() noexcept { return mGroup.c_str(); }

		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;

	public:
		void set_volume(float volume = 0.5f) noexcept;
		void set_position(Vector<float> pos = Vector<float>(0.0f, 0.0f, 0.0f)) noexcept;

		void should_loop(bool enable = true) noexcept;

	private:
		float mVolume{ 0.5f };
		float mPitch{ 100.f };
        bool mLoop{ false };
		float mPan{ 0 };

	private:
		std::vector<std::shared_ptr<Audio::XAudioEngine::XAudioHandle>> mAudios;
		String mGroup;
		String mName;

	};
}
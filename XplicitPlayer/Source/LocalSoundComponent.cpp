/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalSoundComponent.h"

namespace Xplicit::Player
{
	const char* LocalSoundComponent::name() noexcept { return "LocalSoundComponent"; }

	COMPONENT_TYPE LocalSoundComponent::type() noexcept { return COMPONENT_SOUND; }

	bool LocalSoundComponent::should_update() noexcept { return false; }

	PHYSICS_TYPE LocalSoundComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	void LocalSoundComponent::set_position(const Vector<float> pos) noexcept
	{
		Vector<float> mPosition = pos;
	}

	void LocalSoundComponent::set_volume(const float volume) noexcept
	{
		if (volume > 1.0f)
		{
			mVolume = 1.0f;
			return;
		}

		mVolume = volume;
	}

	void LocalSoundComponent::should_loop(const bool enable) noexcept
	{
		mLoop = enable;
	}

	void LocalSoundComponent::play_2d(const String& path) noexcept
	{
		Thread job([&]() {
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(mConverter.from_bytes(path).c_str());
			audio->play(mVolume, 100, 0);
			});

		job.detach();
	}

	void LocalSoundComponent::play(const String& path) noexcept
	{
		Thread job([&]() {
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(mConverter.from_bytes(path).c_str());
			audio->play_3d(mPosition, &mLoop);
		});

		job.detach();
	}
}
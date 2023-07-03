/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
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
		Thread job([](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			audio->play();
		}, path);

		job.detach();
	}

	void LocalSoundComponent::play(const String& path) noexcept
	{
		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			audio->play_3d(mPosition, &mLoop);
		}, path);

		job.detach();
	}
}
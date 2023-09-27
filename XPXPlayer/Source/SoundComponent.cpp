/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "SoundComponent.h"

namespace XPX
{
	SoundComponent::SoundComponent(const char* name, const char* parent)
		: 
		mVolume(1.0f), mLoop(false),
		mPitch(1), mPan(1),
		mGroup(parent), mName(name)
	{
	
	}

	SoundComponent::~SoundComponent()
	{
		for (auto audio : mAudios)
		{
			if (audio)
				audio.reset();
		}
	}

	COMPONENT_TYPE SoundComponent::type() noexcept { return COMPONENT_SOUND; }

	bool SoundComponent::should_update() noexcept { return false; }

	PHYSICS_TYPE SoundComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	void SoundComponent::set_position(Vector<float> pos) noexcept { Vector<float> mPosition = pos; }

	void SoundComponent::set_volume(float volume) noexcept
	{
		if (volume > 1.0f)
		{
			mVolume = 1.0f;
			return;
		}

		mVolume = volume;
	}

	void SoundComponent::update(ClassPtr class_ptr) {}

	void SoundComponent::should_loop(bool enable) noexcept
	{
		mLoop = enable;
	}

	void SoundComponent::play(const String& path) noexcept
	{
#ifdef XPLICIT_WINDOWS
		XPX::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
		std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = XPX::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(path).c_str());

		if (audio)
		{
			mAudios.push_back(audio);
			audio->play_3d(this->pos(), mVolume, mPitch, mPan, &mLoop);
		}
#else
        Vector<float> vel(0, 0, 0);

        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_configure_listener(this->pos(), vel, nullptr);
        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_load_wave(path.c_str(), mLoop);
#endif
	}
}
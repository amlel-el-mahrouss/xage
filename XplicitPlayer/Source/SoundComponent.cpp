/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "SoundComponent.h"

namespace Xplicit::Player
{
	SoundComponent::SoundComponent() 
		: ClassComponent(Vector<float>(0.0f, 0.0f, 0.0f),
			Vector<float>(0.0f, 0.0f, 0.0f), 
			Color<float>(0.0f, 0.0f, 0.0f), 
			nullptr,
			"World",
			"SoundFactory"),
		mVolume(1.0f), mLoop(false)
	{
		this->insert("Loop", "false");
		this->insert("Volume", "1");
	}

	SoundComponent::~SoundComponent()
	{
	}

	const char* SoundComponent::name() noexcept { return "SoundComponent"; }

	COMPONENT_TYPE SoundComponent::type() noexcept { return COMPONENT_SOUND; }

	bool SoundComponent::should_update() noexcept { return true; }

	PHYSICS_TYPE SoundComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	void SoundComponent::set_position(const Vector<float> pos) noexcept { Vector<float> mPosition = pos; }

	void SoundComponent::set_volume(const float volume) noexcept
	{
		if (volume > 1.0f)
		{
			mVolume = 1.0f;
			return;
		}

		mVolume = volume;
	}

	void SoundComponent::update(void* class_ptr)
	{
		ClassComponent::update(class_ptr);
		SoundComponent* _this = (SoundComponent*)class_ptr;

		_this->mLoop = _this->index_as_bool("Loop");
		_this->mVolume = _this->index_as_number<float>("Volume");
	}

	void SoundComponent::should_loop(const bool enable) noexcept
	{
		mLoop = enable;
	}

	void SoundComponent::play_2d(const String& path) noexcept
	{
		Xplicit::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			
			if (audio)
				audio->play();
		}, path);

		job.detach();
	}

	void SoundComponent::play(const String& path) noexcept
	{
		Xplicit::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			
			if (audio)
				audio->play_3d(this->pos(), &mLoop);
		}, path);

		job.detach();
	}
}
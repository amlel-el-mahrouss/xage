/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalSoundComponent.h"
#include <ClassComponent.h>

namespace Xplicit::Player
{
	LocalSoundComponent::LocalSoundComponent() 
		: ClassComponent(Vector<float>(0.0f, 0.0f, 0.0f),
			Vector<float>(0.0f, 0.0f, 0.0f), 
			Color<float>(0.0f, 0.0f, 0.0f), 
			nullptr,
			"World.",
			"SoundMixer"),
		mVolume(0.5f), mLoop(false)
	{
		this->insert("Loop", "false");
		this->insert("Volume", "0.5");
	}

	LocalSoundComponent::~LocalSoundComponent()
	{
	}

	const char* LocalSoundComponent::name() noexcept { return "LocalSoundComponent"; }

	COMPONENT_TYPE LocalSoundComponent::type() noexcept { return COMPONENT_SOUND; }

	bool LocalSoundComponent::should_update() noexcept { return true; }

	PHYSICS_TYPE LocalSoundComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	void LocalSoundComponent::set_position(const Vector<float> pos) noexcept { Vector<float> mPosition = pos; }

	void LocalSoundComponent::set_volume(const float volume) noexcept
	{
		if (volume > 1.0f)
		{
			mVolume = 1.0f;
			return;
		}

		mVolume = volume;
	}

	void LocalSoundComponent::update(void* class_ptr)
	{
		ClassComponent::update(class_ptr);
		LocalSoundComponent* _this = (LocalSoundComponent*)class_ptr;

		_this->mLoop = _this->index_as_number<float>("Loop", 7);
		_this->mVolume = _this->index_as_number<float>("Volume", 8);
	}

	void LocalSoundComponent::should_loop(const bool enable) noexcept
	{
		mLoop = enable;
	}

	void LocalSoundComponent::play_2d(const String& path) noexcept
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

	void LocalSoundComponent::play(const String& path) noexcept
	{
		Xplicit::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			
			if (audio)
				audio->play_3d(this->get_attribute().pos(), &mLoop);
		}, path);

		job.detach();
	}
}
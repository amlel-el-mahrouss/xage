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
		: ClassComponent(Vector<float>(0.0f, 0.0f, 0.0f),
			Vector<float>(0.0f, 0.0f, 0.0f), 
			Color<float>(0.0f, 0.0f, 0.0f), 
			nullptr,
			parent,
			name),
		mVolume(1.0f), mLoop(false)
	{
		this->insert("Loop", "false");
		this->insert("Pitch", "1");
		this->insert("Pan", "1");
		this->insert("Position", "{ X= 0, Y = 0, Z = 0}");
		this->insert("Volume", "1");
		this->insert("Play", "function(self, path) XPXPlaySound(path); end");
	}

	SoundComponent::~SoundComponent() = default;

	const char* SoundComponent::name() noexcept { return "SoundComponent"; }

	COMPONENT_TYPE SoundComponent::type() noexcept { return COMPONENT_SOUND; }

	bool SoundComponent::should_update() noexcept { return true; }

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

	void SoundComponent::update(ClassPtr class_ptr)
	{
		ClassComponent::update(class_ptr);
		SoundComponent* self = (SoundComponent*)class_ptr;

		self->mLoop = self->index_as_bool("Loop");
		self->mPan = self->index_as_number<float>("Pan");
		self->mPitch = self->index_as_number<float>("Pitch");
		self->mVolume = self->index_as_number<float>("Volume");
	}

	void SoundComponent::should_loop(bool enable) noexcept
	{
		mLoop = enable;
	}

	void SoundComponent::play_2d(const String& path) noexcept
	{
#ifdef __XPLICIT_WINDOWS__
        XPX::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path, float vol, float pitch, float pan) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = XPX::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());

			if (audio)
				audio->play(vol, pitch, pan);
		}, path, mVolume, mPitch, mPan);

		job.detach();
#else
        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_load_wave(path.c_str(), mLoop);
#endif
	}

	void SoundComponent::play(const String& path) noexcept
	{
#ifdef __XPLICIT_WINDOWS__
		XPX::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = XPX::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			
			if (audio)
				audio->play_3d(this->pos(), mVolume, mPitch, mPan, &mLoop);
		}, path);

		job.detach();
#else
        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_configure_listener(this->pos(), , nullptr);
        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_load_wave(path.c_str(), mLoop);
#endif
	}
}
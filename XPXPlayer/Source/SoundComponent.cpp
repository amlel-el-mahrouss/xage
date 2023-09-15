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
	static String sound_destroy(const char* name, const char* parent) noexcept
	{
		String func_proto = fmt::format("function(self) destroySound(\"{}\", \"{}\"); end", name, parent);
		return func_proto;
	}

	SoundComponent::SoundComponent(const char* name, const char* parent)
		: ClassComponent(Vector<NetworkFloat>(0.0f, 0.0f, 0.0f),
			Vector<NetworkFloat>(0.0f, 0.0f, 0.0f),
			Color<NetworkFloat>(0.0f, 0.0f, 0.0f),
			nullptr,
			parent,
			name),
		mVolume(1.0f), mLoop(false),
		mPitch(1), mPan(1),
		mGroup(parent), mName(name)
	{
		this->insert("Loop", "false");
		this->insert("Pitch", "0");
		this->insert("Pan", "1");
		
		this->insert("Volume", "1");
		this->insert("SoundId", "''");

		this->insert("Play", "function(self) playSound(self.ClassName, self.SoundId); end");

		this->insert("Destroy", sound_destroy(name, parent).c_str());
	}

	SoundComponent::~SoundComponent() = default;

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

	void SoundComponent::update(ClassPtr class_ptr)
	{

	}

	void SoundComponent::should_loop(bool enable) noexcept
	{
		mLoop = enable;
	}

	void SoundComponent::play(const String& path) noexcept
	{
		this->mLoop = this->index_as_bool("Loop");
		this->mPan = this->index_as_number<float>("Pan");
		this->mPitch = this->index_as_number<float>("Pitch");
		this->mVolume = this->index_as_number<float>("Volume");

#ifdef XPLICIT_WINDOWS
		XPX::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
		std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = XPX::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(path).c_str());

		if (audio)
			audio->play_3d(this->pos(), mVolume, mPitch, mPan, &mLoop);
#else
        Vector<float> vel(0, 0, 0);

        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_configure_listener(this->pos(), vel, nullptr);
        XPX::Audio::XAudioEngine::get_singleton_ptr()->openal_load_wave(path.c_str(), mLoop);
#endif
	}
}
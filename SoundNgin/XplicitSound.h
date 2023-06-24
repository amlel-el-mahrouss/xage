/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitAudio)
 *			Copyright Xplicit Corporation, all rights reserved.
 * 
 *			Purpose: Xaudio backend for Xplicit.
 *
 * =====================================================================
 */

#pragma once

#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <Audio.h>

#ifndef XPLICIT_AUDIO_RATE
#	define XPLICIT_AUDIO_RATE (44100)
#endif // ifndef XPLICIT_AUDIO_RATE

namespace Xplicit
{
	namespace Audio
	{
		class XAudioEngine final
		{
		public:
			explicit XAudioEngine()
			{
				DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef XPLICIT_DEBUG
				eflags |= DirectX::AudioEngine_Debug;
#endif

				try
				{
					mAudioNgin = std::make_unique<DirectX::AudioEngine>(eflags);
				}
				catch (const std::runtime_error& err)
				{
					std::string error = "ERROR: ";
					error += err.what();

					XPLICIT_CRITICAL(error);

					return;
				}

				auto enumList = DirectX::AudioEngine::GetRendererDetails();

				if (enumList.empty())
				{
					// No audio devices
				}
				else
				{
					for (const auto& it : enumList)
					{
						std::cout << "Audio device detected\n";
						std::wcout << it.deviceId.c_str() << std::endl;
						std::wcout << it.description.c_str() << std::endl;
					}
				}

				mAudioNgin->SetDefaultSampleRate(44100);
			}

			~XAudioEngine() = default;

		public:
			struct XAudioHandle final
			{
			public:
				XAudioHandle(DirectX::AudioEngine* engine, const wchar_t* path)
					: mAudio(std::make_unique<DirectX::SoundEffect>(engine, path))
				{
					assert(mAudio);
				}

			public:
				void operator()() noexcept
				{
					mAudio->Play();
				}

				void operator()(const float volume, const float pitch, const float pan) noexcept
				{
					mAudio->Play(volume, pitch, pan);
				}

			private:
				std::unique_ptr<DirectX::SoundEffect> mAudio;

			};

			void update() noexcept
			{
				if (!mAudioNgin->Update())
				{
					// No audio device is active
					if (mAudioNgin->IsCriticalError())
					{
						std::cout << "No audio device active!\n";
					}
				}
			}

			std::unique_ptr<XAudioHandle> make_audio(const wchar_t* path) noexcept
			{
				if (!mAudioNgin)
					return {};

				std::unique_ptr<XAudioHandle> effect = std::make_unique<XAudioHandle>(mAudioNgin.get(), path);
				return effect;
			}

			void set_volume(const float volume) noexcept
			{
				if (!mAudioNgin)
					return;

				if (volume > 1.0 ||
					volume < 0.0)
					return;

				mAudioNgin->SetMasterVolume(volume);
			}

			void set_reverb_quarry() noexcept
			{
				if (!mAudioNgin)
					return;

				mAudioNgin->SetReverb(DirectX::Reverb_Quarry);
			}

			void disable_reverb() noexcept
			{
				if (!mAudioNgin)
					return;

				mAudioNgin->SetReverb(DirectX::Reverb_Off);
			}

			void set_reverb_cave() noexcept
			{
				if (!mAudioNgin)
					return;

				mAudioNgin->SetReverb(DirectX::Reverb_Cave);
			}

			void set_reverb_alley() noexcept
			{
				if (!mAudioNgin)
					return;

				mAudioNgin->SetReverb(DirectX::Reverb_Alley);
			}

			void set_reverb_city() noexcept
			{
				if (!mAudioNgin)
					return;

				mAudioNgin->SetReverb(DirectX::Reverb_City);
			}

			void set_reverb_forest() noexcept
			{
				if (!mAudioNgin)
					return;

				mAudioNgin->SetReverb(DirectX::Reverb_Forest);
			}

			void suspend() noexcept
			{
				if (mAudioNgin)
					mAudioNgin->Suspend();

			}

			void resume() noexcept
			{
				if (mAudioNgin)
					mAudioNgin->Resume();

			}

		private:
			std::unique_ptr<DirectX::AudioEngine> mAudioNgin;

		};
	}
}
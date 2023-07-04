/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitAudio)
 *			Copyright Xplicit Corporation, all rights reserved.
 * 
 *			Purpose: Xplicit Sound backend.
 *
 * =====================================================================
 */

#pragma once

#ifndef __XPLICIT_H__
#	error Please include Xplicit.h before XplicitSound!
#endif // __XPLICIT_H__

#include <iostream>
#include <string>

#define _USE_MATH_DEFINES

#include <Audio.h>
#include <SimpleMath.h>

#ifndef XPLICIT_AUDIO_RATE
#	define XPLICIT_AUDIO_RATE (44100)
#endif // ifndef XPLICIT_AUDIO_RATE

namespace Xplicit
{
	namespace Audio
	{
		class XAudioEngine final
		{
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
					throw EngineError("No audio devices detected!");
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

		public:
			~XAudioEngine() = default;

		public:
			struct XAudioHandle final
			{
			public:
				XAudioHandle(DirectX::AudioEngine* engine, const wchar_t* path)
					: 
					mAudio(std::make_unique<DirectX::SoundEffect>(engine, path)),
					mPos(0.f, 0.0f, 0.0f),
					mEmitter(),
					mListener()
				{
					assert(mAudio);
				}

			public:
				void play() noexcept
				{
					mAudio->Play();
				}

				void play(const float volume, const float pitch, const float pan) noexcept
				{
					mAudio->Play(volume, pitch, pan);
				}

				void play_3d(const Vector<float>& pos, bool* loop = nullptr) noexcept
				{
					if (!mAudio)
						return;

					mSource = mAudio->CreateInstance(DirectX::SoundEffectInstance_Use3D);
					if (!mAudio) return;

					mSource->Play(loop);

					if (loop && 
						*loop)
					{
						// Let this thread handle sound looping.
						Thread thread_job([&]() {
							while (mSource)
							{
								if (!loop)
								{
									mSource->Stop(true);
									break;
								}
							}
							});

						thread_job.detach();
					}
				}

			private:
				std::unique_ptr<DirectX::SoundEffectInstance> mSource;
				std::unique_ptr<DirectX::SoundEffect> mAudio;

			private:
				DirectX::AudioListener mListener;
				DirectX::AudioEmitter  mEmitter;
				Vector<float> mPos;

			private:
				// accessed by friend class.
				bool should_play;

			private:
				friend XAudioEngine;

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
				else
				{
					for (std::size_t index = 0; index < mHandles.size(); ++index)
					{
						if (!mHandles[index])
							continue;

						mHandles[index]->mEmitter.Update(DirectX::FXMVECTOR({ mHandles[index]->mPos.X, mHandles[index]->mPos.Y, mHandles[index]->mPos.Z }),
							DirectX::SimpleMath::Vector3::Up,
							static_cast<float>(std::chrono::steady_clock::duration().count()));

						if (mHandles[index]->mSource)
						{
							mHandles[index]->mSource->Apply3D(mHandles[index]->mListener, mHandles[index]->mEmitter);
						}
					}
				}
			}

			static XAudioEngine* get_singleton_ptr() noexcept
			{
				static XAudioEngine* ngin = nullptr;

				if (ngin == nullptr)
					ngin = new XAudioEngine();

				return ngin;
			}

			std::shared_ptr<XAudioHandle> make_audio(const wchar_t* path) noexcept
			{
				if (!mAudioNgin)
					return {};

				std::shared_ptr<XAudioHandle> effect = std::make_unique<XAudioHandle>(mAudioNgin.get(), path);
				mHandles.push_back(effect);

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
			std::vector<std::shared_ptr<XAudioHandle>> mHandles;
			std::unique_ptr<DirectX::AudioEngine> mAudioNgin;

		private:
			Timer mTimer;
			
		};
	}
}
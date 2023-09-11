/*
 * =====================================================================
 *
 *			XPXNgin (XplicitAudio)
 *			Copyright Xplicit Corporation, all rights reserved.
 * 
 *			Purpose: Xplicit Sound backend.
 *
 * =====================================================================
 */

#pragma once

#ifndef __XPLICIT_H__
#	error Please include XPXNginCore.h before XplicitSound!
#endif // __XPLICIT_H__

#include <iostream>
#include <string>

#define _USE_MATH_DEFINES

#ifndef XPLICIT_AUDIO_RATE
#	define XPLICIT_AUDIO_RATE (44100)
#endif // ifndef XPLICIT_AUDIO_RATE

#ifdef _WIN32

#include <Audio.h>
#include <SimpleMath.h>

namespace XPX
{
	namespace Audio
	{
		class XAudioEngine final
		{
			XAudioEngine()
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

				mAudioNgin->SetDefaultSampleRate(XPLICIT_AUDIO_RATE);
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

				void play_3d(const Vector<float>& pos, const float volume, const float pitch, const float pan, bool* loop = nullptr) noexcept
				{
					if (!mAudio)
						return;

					mSource = mAudio->CreateInstance(DirectX::SoundEffectInstance_Use3D);
					
					mSource->SetPitch(pitch);
					mSource->SetVolume(volume);
					mSource->SetPan(pan);

					mSource->Play(*loop);
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
				XPLICIT_ASSERT(mAudioNgin);

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

#else

#include <OpenAL/OpenAL.h>

namespace XPX {
    namespace Audio {
        class XAudioEngineOpenAL final {
        public:
            XAudioEngineOpenAL()
                : mDevice(alcOpenDevice(nullptr))
            {
                if (!mDevice)
                    throw EngineError("No Audio devices found!");

                ALboolean enumeration;

                enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");

                if (enumeration == AL_FALSE)
                    throw EngineError("ALC_ENUMERATION_EXT: Not supported by Computer");

                const ALCchar *device = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
                const ALCchar *next = device + 1;
                size_t len = 0;

                fprintf(stdout, "Devices list:\n");
                fprintf(stdout, "----------\n");

                while (device && *device != '\0' && next && *next != '\0') {
                    fprintf(stdout, "%s\n", device);
                    len = strlen(device);
                    device += (len + 1);
                    next += (len + 2);
                }

                fprintf(stdout, "----------\n");

                mContext = alcCreateContext(mDevice, nullptr);

                if (!alcMakeContextCurrent(mContext))
                {
                    throw EngineError("alcCreateContext: Not supported by Computer");
                }

                auto pos = Vector<float>(0, 0, 0);
                auto vel = Vector<float>(0, 0, 0);
                ALfloat ori[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

                this->openal_configure_listener(pos, vel, ori);
                this->openal_generate_source(&mSource);
                this->openal_generate_buffer(&mBuffer);
            }

        public:
            XPLICIT_COPY_DEFAULT(XAudioEngineOpenAL);

        public:
            void openal_load_wave(const char* path, bool loop)
            {
                ALsizei size, freq;
                ALenum format = AL_FORMAT_STEREO16;
                ALvoid *data = nullptr;

                this->openal_load_wave_internal(path, size, freq, format, &data, loop);
            }

            bool openal_generate_buffer(ALuint* buffer)
            {
                alGenBuffers((ALuint)1, buffer);

                try
                {
                    this->try_fatal();
                    return true;
                }
                catch (...)
                {
                    return false;
                }
            }
            
            bool openal_configure_listener(Vector<float>& pos, Vector<float>& vel, ALfloat* ori)
            {
                alListener3f(AL_POSITION, pos.X, pos.Y, pos.Z);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                alListener3f(AL_VELOCITY, vel.X, vel.Y, vel.Z);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                alListenerfv(AL_ORIENTATION, ori);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                return true;
            }

            bool openal_generate_source(ALuint* source)
            {
                alGenSources((ALuint)1, source);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }


                alSourcef(*source, AL_PITCH, 1);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                alSourcef(*source, AL_GAIN, 1);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                alSource3f(*source, AL_POSITION, 0, 0, 0);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                alSource3f(*source, AL_VELOCITY, 0, 0, 0);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                alSourcei(*source, AL_LOOPING, AL_FALSE);

                try
                {
                    this->try_fatal();
                }
                catch (...)
                {
                    return false;
                }

                return true;
            }

        public:
            void openal_load_wave_internal(
                    const char* path,
                    ALsizei size,
                    ALsizei freq,
                    ALenum format,
                    ALvoid *data,
                    bool loop
                    )
            {
                XPLICIT_CRITICAL("openal_load_wave_internal: Unimplemented");
            }

        private:
            bool try_fatal() noexcept
            {
                ALCenum error;

                error = alGetError();

                if (error != AL_NO_ERROR)
                {
                    throw EngineError("OpenAL: Fatal error!");
                }

                return true;
            }

        private:
            ALCcontext* mContext;
            ALCdevice* mDevice;
            ALuint mBuffer;
            ALuint mSource;

        };

        typedef XAudioEngineOpenAL XAudioEngine;
    }
}

#endif

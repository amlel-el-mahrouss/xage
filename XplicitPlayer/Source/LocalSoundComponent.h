/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Component.h>
#include <codecvt>
#include <Util.h>

#include <Xplicit.h>
#include <XplicitSound.h>

namespace Xplicit::Player
{
	class LocalSoundComponent final : public Component
	{
	public:
		explicit LocalSoundComponent() = default;
		~LocalSoundComponent() override = default;

	public:
		XPLICIT_COPY_DEFAULT(LocalSoundComponent);

	public:
		void play(const String& path) noexcept;
		void play_2d(const String& path) noexcept;

	public:
		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		bool should_update() noexcept override;
		PHYSICS_TYPE physics() noexcept override;

		void set_volume(const float volume = 0.5f) noexcept;
		void should_loop(const bool enable = true) noexcept;

	private:
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> mConverter;

	private:
		Vector<float> mPosition;
		float mVolume{ 0.5f };
		bool mLoop{ false };

	};
}
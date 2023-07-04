/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Player
{
	class LocalMenuEvent final : public Event
	{
	public:
		LocalMenuEvent() = delete;

	public:
		explicit LocalMenuEvent() noexcept;
		~LocalMenuEvent() override;

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

	public:
		void enable(const bool enable = false) noexcept;
		bool enabled() const noexcept;

	public:
		void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		ITexture* mButtonNoHover;
		ITexture* mButtonHover;
		ITexture* mMenu;

	private:
		std::int64_t mTimeout;
		bool mEnabled;

	};
}
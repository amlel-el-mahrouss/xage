/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Player
{
	class XPLICIT_API LocalMenuEvent final : public Event
	{
	public:
		explicit LocalMenuEvent(const std::int64_t& hash);
		~LocalMenuEvent() override;

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

		void enable(const bool enable = false) noexcept;
		bool enabled() const noexcept;

		void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		ITexture* mButtonNoHover;
		ITexture* mButtonHover;
		std::int64_t mHash;
		int64_t mTimeout;
		ITexture* mMenu;

		/* logic states */
		bool mShutdown; // is shutdown requested?
		bool mEnabled; // is menu enabled?

	};
}
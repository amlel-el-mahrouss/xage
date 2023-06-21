/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalMenuEvent.h
 *			Purpose: Client menu
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
		explicit LocalMenuEvent();
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
		int64_t mTimeout;
		ITexture* mMenu;
		bool mEnabled;

	};
}
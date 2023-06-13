/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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
		LocalMenuEvent(const int64_t& hash);
		virtual ~LocalMenuEvent();

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

		void enable(const bool enable = false) noexcept;
		bool enabled() noexcept;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* m_network;
		int64_t m_timeout;
		ITexture* m_menu;
		bool m_enabled;
		int64_t m_hash;

	};
}
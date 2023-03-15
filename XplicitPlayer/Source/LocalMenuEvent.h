/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalMenuEvent.h
 *			Purpose: CoreUI menu.
 *
 * =====================================================================
 */

#pragma once

#include <ApplicationContext.h>
#include <NetworkInstance.h>
#include <Event.h>

namespace Xplicit::Client
{
	class XPLICIT_API LocalMenuEvent : public Event
	{
	public:
		LocalMenuEvent(const int64_t& id);
		virtual ~LocalMenuEvent();

		LocalMenuEvent& operator=(const LocalMenuEvent&) = default;
		LocalMenuEvent(const LocalMenuEvent&) = default;

		void enable(const bool enable = false) noexcept;
		bool enabled() noexcept;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkInstance* m_network;
		int64_t m_timeout;
		ITexture* m_menu;
		bool m_enabled;
		int64_t m_hash;

	};
}
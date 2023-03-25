/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalWatchdogEvent.h
 *			Purpose: Watchdog client
 *
 * =====================================================================
 */

#pragma once

#include <ApplicationContext.h>
#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Client
{
	class LocalWatchdogEvent final : public Event
	{
	public:
		LocalWatchdogEvent() = delete;

	public:
		LocalWatchdogEvent(const int64_t& id);
		~LocalWatchdogEvent();

		LocalWatchdogEvent& operator=(const LocalWatchdogEvent&) = default;
		LocalWatchdogEvent(const LocalWatchdogEvent&) = default;

		virtual const char* name() noexcept override { return ("LocalWatchdogEvent"); }
		virtual void operator()() override;

	private:
		NetworkComponent* m_network;
		int64_t m_id;

	};
}
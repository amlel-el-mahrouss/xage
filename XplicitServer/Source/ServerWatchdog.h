/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: ServerWatchdog.h
 *			Purpose: Server Watchdog
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	class ServerWatchdogEvent : public Event
	{
	public:
		ServerWatchdogEvent();
		virtual ~ServerWatchdogEvent();

		ServerWatchdogEvent& operator=(const ServerWatchdogEvent&) = default;
		ServerWatchdogEvent(const ServerWatchdogEvent&) = default;

		virtual const char* name() noexcept override;
		virtual void operator()() override;

	private:
		NetworkServerComponent* m_server; // Wacthdog ServerComponent.
		int64_t m_watchdog_timer; // Watchdog delay

	};
}
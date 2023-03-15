/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalResetEvent.h
 *			Purpose: RST timeout event.
 *
 * =====================================================================
 */

#pragma once

#include <ApplicationContext.h>
#include <NetworkInstance.h>
#include <Event.h>

namespace Xplicit::Client
{
	class XPLICIT_API LocalResetEvent : public Event
	{
	public:
		LocalResetEvent();
		virtual ~LocalResetEvent();

		LocalResetEvent& operator=(const LocalResetEvent&) = default;
		LocalResetEvent(const LocalResetEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkInstance* m_network;
		int32_t m_num_resets;

	};
}
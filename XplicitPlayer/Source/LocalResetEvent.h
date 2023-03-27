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

#include <NetworkComponent.h>
#include <Event.h>

namespace Xplicit::Client
{
	class XPLICIT_API LocalResetEvent final : public Event
	{
	public:
		LocalResetEvent() = delete;

	public:
		LocalResetEvent(int64_t hash);
		virtual ~LocalResetEvent();

		LocalResetEvent& operator=(const LocalResetEvent&) = default;
		LocalResetEvent(const LocalResetEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* m_network;
		int32_t m_resets;
		int64_t m_hash;

	};
}
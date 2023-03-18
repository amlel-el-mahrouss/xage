/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event (duh)
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

#include "Actor.h"
#include "ServerWatchdog.h"

namespace Xplicit
{
	class PlayerJoinLeaveEvent : public Event
	{
	public:
		PlayerJoinLeaveEvent();
		virtual ~PlayerJoinLeaveEvent();

		PlayerJoinLeaveEvent& operator=(const PlayerJoinLeaveEvent&) = default;
		PlayerJoinLeaveEvent(const PlayerJoinLeaveEvent&) = default;

		virtual void operator()() override;
		virtual const char* name() noexcept override;

	public:
		const size_t& size() noexcept;

	private:
		bool on_leave(NetworkServerComponent* server) noexcept;
		bool on_join(NetworkServerComponent* server) noexcept;

	private:
		size_t m_size;

	};
}
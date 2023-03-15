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
		bool leave_event(NetworkServerInstance* server) noexcept;
		bool join_event(NetworkServerInstance* server, size_t peer_idx) noexcept;

	private:
		bool m_locked;
		size_t m_size;

	};
}
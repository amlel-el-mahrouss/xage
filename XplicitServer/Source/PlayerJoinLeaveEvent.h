/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Join and Leave Event
 *
 * =====================================================================
 */

#pragma once

#include "CommonInc.h"
#include "PlayerComponent.h"

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
		bool handle_leave_events(NetworkServerComponent* server) noexcept;
		bool handle_join_events(NetworkServerComponent* server) noexcept;

	private:
		size_t mPlayerCount;

	};
}
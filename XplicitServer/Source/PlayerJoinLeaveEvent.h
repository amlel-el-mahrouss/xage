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

	public:
		PlayerJoinLeaveEvent& operator=(const PlayerJoinLeaveEvent&) = default;
		PlayerJoinLeaveEvent(const PlayerJoinLeaveEvent&) = default;

	public:
		virtual const char* name() noexcept override;
		virtual void operator()() override;

	public:
		const size_t& size() noexcept;

	private:
		bool handle_leave_event(NetworkServerComponent* server) noexcept;
		bool handle_join_event(NetworkServerComponent* server) noexcept;

	private:
		size_t mPlayerCount;

	};
}
/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"
#include "PlayerComponent.h"

namespace Xplicit
{
	class PlayerJoinLeaveEvent final : public Event
	{
	public:
		PlayerJoinLeaveEvent();
		~PlayerJoinLeaveEvent() override;
		
		PlayerJoinLeaveEvent& operator=(const PlayerJoinLeaveEvent&) = default;
		PlayerJoinLeaveEvent(const PlayerJoinLeaveEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;
		
		const size_t& size() noexcept;

	private:
		bool handle_leave_event() noexcept;
		bool handle_join_event() noexcept;
		
		std::vector<PlayerComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
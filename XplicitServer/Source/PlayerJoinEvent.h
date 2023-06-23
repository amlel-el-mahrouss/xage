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

#define XPLICIT_XASSET_ENDPOINT ("play-xplicit.com/assets")

namespace Xplicit
{
	class PlayerJoinEvent final : public Event
	{
	public:
		PlayerJoinEvent();
		~PlayerJoinEvent() override;
		
		PlayerJoinEvent& operator=(const PlayerJoinEvent&) = default;
		PlayerJoinEvent(const PlayerJoinEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;
		
		const size_t& size() const noexcept;

	private:
		void handle_leave_event() noexcept;
		void handle_join_event() noexcept;

		std::vector<PlayerComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
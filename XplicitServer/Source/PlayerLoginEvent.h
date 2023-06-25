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
#include "HumanoidComponent.h"

#define XPLICIT_XASSET_ENDPOINT ("play-xplicit.com/assets")

namespace Xplicit
{
	class PlayerLoginEvent final : public Event
	{
	public:
		PlayerLoginEvent();
		~PlayerLoginEvent() override;
		
		PlayerLoginEvent& operator=(const PlayerLoginEvent&) = default;
		PlayerLoginEvent(const PlayerLoginEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;
		
		const size_t& size() const noexcept;

	private:
		void handle_leave_event() noexcept;
		void handle_join_event() noexcept;

		std::vector<HumanoidComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
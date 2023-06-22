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
	class PlayerLeaveEvent final : public Event
	{
	public:
		PlayerLeaveEvent();
		~PlayerLeaveEvent() override;
		
		PlayerLeaveEvent& operator=(const PlayerLeaveEvent&) = default;
		PlayerLeaveEvent(const PlayerLeaveEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;
		
		const size_t& size() const noexcept;

	private:
		std::vector<PlayerComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
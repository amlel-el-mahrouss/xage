/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"
#include "HumanoidComponent.h"

namespace Xplicit
{
	class PlayerLoginEvent final : public Event
	{
	public:
		explicit PlayerLoginEvent();
		~PlayerLoginEvent() override;
		
		PlayerLoginEvent& operator=(const PlayerLoginEvent&) = default;
		PlayerLoginEvent(const PlayerLoginEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;
		
		const size_t& size() const noexcept;

	private:
		void handle_leave_event() noexcept;
		void handle_join_event() noexcept;

	private:
		std::vector<HumanoidComponent*> mPlayers;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "HumanoidComponent.h"
#include "ServerReplicationFactory.h"

namespace Xplicit
{
	class HumanoidReplicationEvent final : public Event
	{
	public:
		explicit HumanoidReplicationEvent();
		~HumanoidReplicationEvent() override;

	public:
		HumanoidReplicationEvent& operator=(const HumanoidReplicationEvent&) = default;
		HumanoidReplicationEvent(const HumanoidReplicationEvent&) = default;

	public:
		const char* name() noexcept override;
		void operator()() override;

	public:
		const size_t& size() const noexcept;

	private:
		std::vector<HumanoidComponent*> mPlayers;
		ServerReplicationFactory mFactory;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
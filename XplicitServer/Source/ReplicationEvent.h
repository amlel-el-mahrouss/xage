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
	class ReplicationEvent final : public Event
	{
	public:
		explicit ReplicationEvent();
		~ReplicationEvent() override;

	public:
		ReplicationEvent& operator=(const ReplicationEvent&) = default;
		ReplicationEvent(const ReplicationEvent&) = default;

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
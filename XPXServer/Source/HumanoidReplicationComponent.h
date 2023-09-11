/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "HumanoidComponent.h"
#include "ServerReplicationFactory.h"

namespace XPX
{
	class HumanoidReplicationComponent final : public Component
	{
	public:
		explicit HumanoidReplicationComponent();
		~HumanoidReplicationComponent() override;

	public:
		HumanoidReplicationComponent& operator=(const HumanoidReplicationComponent&) = default;
		HumanoidReplicationComponent(const HumanoidReplicationComponent&) = default;

	public:
		const char* name() noexcept override;

		static bool should_update() noexcept;
		static void update(ClassPtr ptr);

	public:
		const size_t& size() const noexcept;

	private:
		std::vector<HumanoidComponent*> mPlayers;
		ServerReplicationFactory mFactory;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}
/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: PlayerSpawnDeathEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "SpawnComponent.h"
#include "HumanoidComponent.h"

namespace Xplicit
{
	class PlayerSpawnDeathEvent : public Event
	{
	public:
		PlayerSpawnDeathEvent() 
			: mNetwork(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
			  mSpawner(ComponentManager::get_singleton_ptr()->get<SpawnComponent>("SpawnComponent"))
		{
			XPLICIT_ASSERT(mNetwork);
		}

		virtual ~PlayerSpawnDeathEvent() = default;

		PlayerSpawnDeathEvent& operator=(const PlayerSpawnDeathEvent&) = default;
		PlayerSpawnDeathEvent(const PlayerSpawnDeathEvent&) = default;

		virtual const char* name() noexcept override { return ("PlayerSpawnDeathEvent"); }

		virtual void operator()() override;

	private:
		std::vector<HumanoidComponent*> mDeadPlayers;
		NetworkServerComponent* mNetwork;
		SpawnComponent* mSpawner;

	};
}
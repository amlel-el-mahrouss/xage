/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: PlayerSpawnDeathEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "SpawnComponent.h"
#include "PlayerComponent.h"

namespace Xplicit
{
	class PlayerSpawnDeathEvent : public Event
	{
	public:
		PlayerSpawnDeathEvent() 
			: m_network(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
			  mSpawner(ComponentManager::get_singleton_ptr()->get<SpawnComponent>("SpawnComponent"))
		{
			XPLICIT_ASSERT(m_network);
			XPLICIT_ASSERT(mSpawner);
		}

		virtual ~PlayerSpawnDeathEvent() = default;

		PlayerSpawnDeathEvent& operator=(const PlayerSpawnDeathEvent&) = default;
		PlayerSpawnDeathEvent(const PlayerSpawnDeathEvent&) = default;

		virtual const char* name() noexcept override { return ("PlayerSpawnDeathEvent"); }

		virtual void operator()() override;

	private:
		std::vector<PlayerComponent*> mDeadActors;
		NetworkServerComponent* m_network;
		SpawnComponent* mSpawner;

	};
}
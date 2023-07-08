/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: PlayerSpawnDeathEvent.cpp
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "PlayerSpawnDeathEvent.h"
#include <lua/CLua.hpp>

namespace Xplicit
{
	constexpr int16_t XPLICIT_DEATH_DELAY = 5;

	/// <summary>
	/// Handle Spawn at a specific point.
	/// </summary>
	/// <param name="spawner">The Spawn component</param>
	/// <param name="humanoid">The Targeted Humanoid</param>
	/// <returns></returns>
	
	static void XplicitHandleSpawn(SpawnComponent* spawner, HumanoidComponent* humanoid) noexcept
	{
		if (humanoid && spawner)
		{
			const auto pos = spawner->get();

			humanoid->get_attribute().pos().X = pos.X;
			humanoid->get_attribute().pos().Y = pos.Y;
			humanoid->get_attribute().pos().Z = pos.Z;
		}
		else if (humanoid)
		{
			humanoid->get_attribute().pos().X = 0.f;
			humanoid->get_attribute().pos().Y = 0.f;
			humanoid->get_attribute().pos().Z = 0.f;
		}
	}

	void PlayerSpawnDeathEvent::operator()()
	{
		if (!mNetwork)
			return;

		for (const auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent"); 
			HumanoidComponent* humanoid : humanoids)
		{
			if (humanoid == nullptr ||
				humanoid->get_peer() == nullptr ||
				humanoid->get_state() == HUMANOID_STATE::DEAD)
				continue;
			
			auto* peer_ptr = humanoid->get_peer();

			if (peer_ptr->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			peer_ptr->packet.health = humanoid->get_health();

			if (!humanoid->is_alive())
			{
				humanoid->should_spawn(false);
				humanoid->set_state(HUMANOID_STATE::DEAD);

#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("Game:Death [EVENT]");
#endif // ifdef XPLICIT_DEBUG

				Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Death()");

				mDeadPlayers.push_back(humanoid);

				for (size_t peer = 0; peer < mNetwork->size(); ++peer)
				{
					peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
					peer_ptr->packet.public_hash = humanoid->get_peer()->public_hash;
				}

				Thread respawn_job([&](HumanoidComponent* humanoid) {
					std::this_thread::sleep_for(std::chrono::seconds(XPLICIT_DEATH_DELAY));

					if (humanoid)
					{
						humanoid->set_health(XPLICIT_DEFAULT_HEALTH);
						humanoid->set_state(HUMANOID_STATE::ALIVE);
					}
				}, humanoid);

				respawn_job.detach();
			}
			else
			{
				auto it = std::find(mDeadPlayers.cbegin(), mDeadPlayers.cend(), humanoid);

				if (it != mDeadPlayers.cend())
				{
					mDeadPlayers.erase(it);
					
					for (size_t peer = 0; peer < mNetwork->size(); ++peer)
					{
						peer_ptr->packet.public_hash = humanoid->get_peer()->public_hash;
						peer_ptr->packet.health = humanoid->get_health();
					}

					XplicitHandleSpawn(mSpawner, humanoid);

					humanoid->should_spawn(true);

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("Game:Spawn [EVENT]");
#endif // ifdef XPLICIT_DEBUG


					Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Spawn()");
				}
			}
		}
	}
}
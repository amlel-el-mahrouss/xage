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
	/// Handle spawn location.
	/// </summary>
	/// <param name="spawner">Spawn component</param>
	/// <param name="humanoid">Target Humanoid</param>
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

		static auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");

		for (HumanoidComponent* humanoid : humanoids)
		{
			if (humanoid == nullptr ||
				humanoid->get_peer() == nullptr ||
				!humanoid->can_spawn())
				continue;
			
			auto* peer_ptr = humanoid->get_peer();

			if (peer_ptr->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			peer_ptr->packet.health = humanoid->get_health();

			if (!humanoid->is_alive() &&
				humanoid->can_spawn())
			{
				humanoid->can_spawn(false);
				humanoid->set_state(HUMANOID_STATE::DEAD);

#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("Game:Death [EVENT]");
#endif // ifdef XPLICIT_DEBUG

				Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Death()");

				for (std::size_t peer = 0UL; peer < mNetwork->size(); ++peer)
				{
					peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
					peer_ptr->packet.public_hash = humanoid->get_peer()->public_hash;
				}

				Thread respawn_job([&](HumanoidComponent* humanoid) {
					std::this_thread::sleep_for(std::chrono::seconds(XPLICIT_DEATH_DELAY));

					if (humanoid &&
						humanoid->get_peer())
					{
						humanoid->set_health(XPLICIT_DEFAULT_HEALTH);
						humanoid->set_state(HUMANOID_STATE::ALIVE);

						XplicitHandleSpawn(mSpawner, humanoid);

#ifdef XPLICIT_DEBUG
						XPLICIT_INFO("Game:Spawn [EVENT]");
#endif // ifdef XPLICIT_DEBUG

						Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Spawn()");
					}
				}, humanoid);

				respawn_job.detach();
			}
		}
	}
}
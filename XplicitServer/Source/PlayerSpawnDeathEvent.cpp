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
#include <lua/lua.hpp>

namespace Xplicit
{
	constexpr int16_t XPLICIT_DEATH_DELAY = 5;

	/// <summary>
	/// Handle Spawn at a specific point.
	/// </summary>
	/// <param name="spawner">The Spawn component</param>
	/// <param name="player">The Targeted Humanoid</param>
	/// <returns></returns>
	
	static void xplicit_handle_spawn(SpawnComponent* spawner, HumanoidComponent* player) noexcept
	{
		if (player && spawner)
		{
			const auto& pos = spawner->get();

			player->get_attribute().pos().X = pos.X;
			player->get_attribute().pos().Y = pos.Y;
			player->get_attribute().pos().Z = pos.Z;
		}
		else if (player)
		{
			player->get_attribute().pos().X = 0.f;
			player->get_attribute().pos().Y = 0.f;
			player->get_attribute().pos().Z = 0.f;
		}
	}

	void PlayerSpawnDeathEvent::operator()()
	{
		if (!mNetwork)
			return;

		for (const auto players = ComponentManager::get_singleton_ptr()->all_of<HumanoidComponent>("Player"); 
			HumanoidComponent* player : players)
		{
			if (player == nullptr ||
				player->get_peer() == nullptr)
				continue;
			
			auto* peer_ptr = player->get_peer();

			if (peer_ptr->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (peer_ptr->packet.health != player->get_health() &&
				peer_ptr->packet.hash == player->get_peer()->hash)
				player->set_health(peer_ptr->packet.health);
			else
				peer_ptr->packet.health = player->get_health();

			if (!player->is_alive() && player->can_spawn())
			{
				player->should_spawn(false);

				if (player->get_attribute().script() &&
					player->get_attribute().script()->name() == "Death")
				{
					player->get_attribute().script()->run();
					XPLICIT_INFO("Humanoid:Death [EVENT]");
				}

				XPLICIT_INFO("Engine:Death [EVENT]");
				Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Death()");

				mDeadPlayers.push_back(player);

				for (size_t peer = 0; peer < mNetwork->size(); ++peer)
				{
					peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
					peer_ptr->packet.public_hash = player->get_peer()->public_hash;
				}

				Thread respawnJob([](HumanoidComponent* humanoid) {
					std::this_thread::sleep_for(std::chrono::seconds(XPLICIT_DEATH_DELAY));

					if (humanoid)
					{
						humanoid->set_health(XPLICIT_DEFAULT_HEALTH);

						if (humanoid->get_attribute().script() &&
							humanoid->get_attribute().script()->name() == "Spawn")
						{
							XPLICIT_INFO("Humanoid:Spawn [EVENT]");
							humanoid->get_attribute().script()->run();
						}

						XPLICIT_INFO("Engine:Spawn [EVENT]");

						Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Spawn()");
					}
				}, player);
			}
			else
			{
				auto it = std::find(mDeadPlayers.cbegin(), mDeadPlayers.cend(), player);

				if (it != mDeadPlayers.cend())
				{
					if (player->can_spawn())
						continue;

					mDeadPlayers.erase(it);
					
					for (size_t peer = 0; peer < mNetwork->size(); ++peer)
					{
						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_INVALID;
						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

						peer_ptr->packet.public_hash = player->get_peer()->public_hash;
						peer_ptr->packet.health = player->get_health();

						xplicit_handle_spawn(mSpawner, player);
					}

					player->should_spawn(true);
				}
			}
		}
	}
}
/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
	/// <param name="spawner">The spawn component</param>
	/// <param name="player">The targeted player</param>
	/// <returns></returns>
	
	static void xplicit_handle_spawn(SpawnComponent* spawner, HumanoidComponent* player) noexcept
	{
		if (player && spawner)
		{
			const auto& pos = spawner->get();

			player->get_pos().X = pos.X;
			player->get_pos().Y = pos.Y;
			player->get_pos().Z = pos.Z;
		}
		else if (player)
		{
			player->get_pos().X = 0.f;
			player->get_pos().Y = 0.f;
			player->get_pos().Z = 0.f;
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

			peer_ptr->packet.health = player->get_health();

			if (!player->is_alive() && player->can_spawn())
			{
				player->should_spawn(false);

				XPLICIT_INFO("Humanoid:Death [EVENT]");
				Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Death()");

				// Place event here (TODO)

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

						XPLICIT_INFO("Humanoid:Spawn [EVENT]");
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

						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

						xplicit_handle_spawn(mSpawner, player);

						peer_ptr->packet.pos[XPLICIT_NETWORK_X] = player->get_pos().X;
						peer_ptr->packet.pos[XPLICIT_NETWORK_Y] = player->get_pos().Y;
						peer_ptr->packet.pos[XPLICIT_NETWORK_Z] = player->get_pos().Z;
					}

					player->should_spawn(true);
				}
			}
		}
	}
}
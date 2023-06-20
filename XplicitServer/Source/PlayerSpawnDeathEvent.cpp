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

namespace Xplicit
{
	constexpr int16_t XPLICIT_DEATH_DELAY = 1000;

	/// <summary>
	/// Handle Spawn at a specific point.
	/// </summary>
	/// <param name="spawner">The spawn component</param>
	/// <param name="actor">The targeted actor</param>
	/// <returns></returns>
	
	static void xplicit_handle_spawn(SpawnComponent* spawner, PlayerComponent* actor) noexcept
	{
		if (actor)
		{
			auto& pos = spawner->get();

			actor->pos().X = pos.X;
			actor->pos().Y = pos.Y;
			actor->pos().Z = pos.Z;
		}
	}

	void PlayerSpawnDeathEvent::operator()()
	{
		auto network = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		if (!network)
			return;

		auto players = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (PlayerComponent* player : players)
		{
			if (player == nullptr)
				continue;

			if (player->get()->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
				continue;

			if (!player->alive())
			{
				// tell everyone that we're dead.
				player->health(0);
				mDeadActors.push_back(player);

				for (size_t peer = 0; peer < m_network->size(); ++peer)
				{
					auto* peer_ptr = m_network->get(peer);
					XPLICIT_ASSERT(peer_ptr);

					peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
					peer_ptr->packet.public_hash = player->get()->public_hash;
				}
			}
			else
			{
				auto it = std::find(mDeadActors.cbegin(), mDeadActors.cend(), player);

				if (it != mDeadActors.cend())
				{
					mDeadActors.erase(it);
					
					for (size_t peer = 0; peer < m_network->size(); ++peer)
					{
						auto* peer_ptr = m_network->get(peer);
						XPLICIT_ASSERT(peer_ptr);

						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_INVALID;

						peer_ptr->packet.public_hash = player->get()->public_hash;
						peer_ptr->packet.health = player->health();

						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

						xplicit_handle_spawn(mSpawner, player);

						peer_ptr->packet.speed[XPLICIT_NETWORK_X] = player->pos().X;
						peer_ptr->packet.speed[XPLICIT_NETWORK_Y] = player->pos().Y;
						peer_ptr->packet.speed[XPLICIT_NETWORK_Z] = player->pos().Z;
					}

				}
			}
		}
	}
}
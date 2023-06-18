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
		auto players = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (PlayerComponent* player : players)
		{
			if (!player)
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

						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

						peer_ptr->packet.public_hash = player->get()->public_hash;
						peer_ptr->packet.health = player->health();

						Xplicit::NetworkServerHelper::send(m_network);
					}

					xplicit_handle_spawn(m_spawner, player);
				}
			}
		}
	}
}
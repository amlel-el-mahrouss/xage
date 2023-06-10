/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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
		auto actors = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (PlayerComponent* actor : actors)
		{
			if (!actor)
				continue;

			if (!actor->alive())
			{
				// tell everyone that we're dead.
				actor->health(0);
				m_dead_actors.push_back(actor);

				for (size_t peer = 0; peer < m_network->size(); ++peer)
				{
					auto* peer_ptr = m_network->get(peer);
					XPLICIT_ASSERT(peer_ptr);

					peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
					peer_ptr->packet.public_hash = actor->get()->public_hash;
				}
			}
			else
			{
				auto it = std::find(m_dead_actors.cbegin(), m_dead_actors.cend(), actor);

				if (it != m_dead_actors.cend())
				{
					m_dead_actors.erase(it);
					
					for (size_t peer = 0; peer < m_network->size(); ++peer)
					{
						auto* peer_ptr = m_network->get(peer);
						XPLICIT_ASSERT(peer_ptr);

						peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;

						peer_ptr->packet.public_hash = actor->get()->public_hash;
						peer_ptr->packet.health = actor->health();
					}

					xplicit_handle_spawn(m_spawner, actor);
				}
			}
		}
	}
}
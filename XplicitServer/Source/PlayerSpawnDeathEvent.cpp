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

#include "PlayerSpawnDeathEvent.h"

namespace Xplicit
{
	constexpr int16_t XPLICIT_DEATH_DELAY = 1000;
	constexpr int16_t XPLICIT_FORCEFIELD_DELAY = 2000;

	void PlayerSpawnDeathEvent::operator()()
	{
		auto actors = InstanceManager::get_singleton_ptr()->get_all<Actor>("Actor");

		for (Actor* actor : actors)
		{
			if (!actor->get())
				continue;

			if (actor->health() <= 0)
			{
				actor->health(0);
				actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;

				m_dead_actors.push_back(actor);
			}
			else
			{
				auto it = std::find(m_dead_actors.cbegin(), m_dead_actors.cend(), actor);

				if (it != m_dead_actors.cend())
				{
					m_dead_actors.erase(it);
					actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] = NETWORK_CMD_SPAWN;
				}
			}
		}
	}
}
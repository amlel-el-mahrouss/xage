/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerMovementEvent.h
 *			Purpose: Player Movement Event
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "PlayerMovementEvent.h"

namespace Xplicit
{
	static bool xplicit_detect_collisions(std::vector<Actor*>& actors, Actor* actor)
	{
		if (!actor ||
			!actor->can_collide())
			return false;

		if (actors.empty())
			return false;

		/*
		for (size_t i = 0; i < actors.size(); i++)
		{

		}
		*/

		return false;
	}

	void PlayerMovementEvent::operator()()
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<Actor>("Actor");

		for (size_t i = 0; i < actors.size(); i++)
		{
			Actor* actor = actors[i];

			if (!actor ||
				actor->health() <= 0 ||
				!actor->get())
				continue;

			NetworkPeer* peer = actor->get();

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS)
			{
				if (!xplicit_detect_collisions(actors, actor))
					peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_ACCEPT;
			}
		}
	}
}
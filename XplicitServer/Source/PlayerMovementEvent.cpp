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
	void PlayerMovementEvent::operator()()
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<Actor>("Actor");

		for (Actor* actor : actors)
		{
			if (!actor ||
				actor->health() <= 0 ||
				!actor->get())
				continue;

			auto peer = actor->get();

			peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

			// TODO: Game Vars, needs to be taken into consideration here.
			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
			{
				peer->packet.X = 10.f;
			}
			else if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
			{
				peer->packet.X = -10.f;
			}
			else if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
			{
				peer->packet.Z = 10.f;
			}
			else if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
			{
				peer->packet.Z = -10.f;
			}
			else
			{
				peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_INVALID;
			}
		}
	}
}
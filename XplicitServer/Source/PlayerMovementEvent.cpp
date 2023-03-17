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
	constexpr int XPLICIT_WALK_SPEED = 10.f;
	constexpr int XPLICIT_SIDE_SPEED = 5.f;

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

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
				peer->packet.X = XPLICIT_WALK_SPEED;
			else if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
				peer->packet.X = -XPLICIT_WALK_SPEED;
			else if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
				peer->packet.Z = -XPLICIT_SIDE_SPEED;
			else if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
				peer->packet.Z = XPLICIT_SIDE_SPEED;
			else
				continue;

			peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
		}
	}
}
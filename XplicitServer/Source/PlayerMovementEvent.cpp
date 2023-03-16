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
		auto actors = InstanceManager::get_singleton_ptr()->all_of<Actor>("Actor");

		for (Actor* actor : actors)
		{
			if (!actor->get() ||
				actor->health() <= 0)
				continue;

			actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

			if (actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
			{
				actor->get()->packet.X = 10.f;
			}
			else if (actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARDS)
			{
				actor->get()->packet.X = -10.f;
			}
			else if (actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
			{
				actor->get()->packet.Z = 10.f;
			}
			else if (actor->get()->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
			{
				actor->get()->packet.Z = -10.f;
			}
		}
	}
}
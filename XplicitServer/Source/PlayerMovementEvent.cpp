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
 * @file
 */

#include "PlayerMovementEvent.h"

namespace Xplicit
{
	PlayerMovementEvent::PlayerMovementEvent() {}
	PlayerMovementEvent::~PlayerMovementEvent() {}

	void PlayerMovementEvent::operator()()
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (size_t i = 0; i < actors.size(); ++i)
		{
			PlayerComponent* actor = actors[i];

			if (!actor ||
				actor->health() <= 0 ||
				!actor->get())
				continue;

			NetworkPeer* peer = actor->get();

			if (peer->public_hash != peer->packet.public_hash)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] != NETWORK_CMD_ACCEPT &&
				!actor->is_frozen()) // here
			{
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					actor->pos().Z += XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					actor->pos().Z -= XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					actor->pos().X -= XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					actor->pos().X += XPLICIT_DEFAULT_VEL;

				peer->packet.speed[XPLICIT_NETWORK_X] = XPLICIT_DEFAULT_VEL;
				peer->packet.speed[XPLICIT_NETWORK_Y] = XPLICIT_DEFAULT_VEL;
				peer->packet.speed[XPLICIT_NETWORK_Z] = XPLICIT_DEFAULT_VEL;

				// not the 'freeze' as you think of, this freezes position commands from being sent to a player, see the previous condition.
				// for a period of time.
				actor->freeze_for(XPLICIT_MOVEMENT_RATE);
			}
			else
			{
				peer->packet.speed[XPLICIT_NETWORK_X] = 0UL;
				peer->packet.speed[XPLICIT_NETWORK_Y] = 0UL;
				peer->packet.speed[XPLICIT_NETWORK_Z] = 0UL;

			}
		}
	}
}

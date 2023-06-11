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
	PlayerMovementEvent::PlayerMovementEvent() = default;
	PlayerMovementEvent::~PlayerMovementEvent() = default;

	const char* PlayerMovementEvent::name() noexcept { return ("PlayerMovementEvent"); }

	void PlayerMovementEvent::operator()()
	{
		auto players = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (size_t i = 0; i < players.size(); ++i)
		{
			PlayerComponent* ply = players[i];

			if (ply == nullptr ||
				ply->health() <= 0 ||
				ply->is_frozen())
				continue;

			NetworkPeer* peer = ply->get();

			if (peer->public_hash != peer->packet.public_hash)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] != NETWORK_CMD_ACCEPT) // here
			{
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					ply->pos().Z += XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					ply->pos().Z -= XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					ply->pos().X -= XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					ply->pos().X += XPLICIT_DEFAULT_VEL;

				peer->packet.speed[XPLICIT_NETWORK_X] = XPLICIT_DEFAULT_VEL;
				peer->packet.speed[XPLICIT_NETWORK_Y] = XPLICIT_DEFAULT_VEL;
				peer->packet.speed[XPLICIT_NETWORK_Z] = XPLICIT_DEFAULT_VEL;

				ply->freeze_for(XPLICIT_MOVEMENT_RATE);
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

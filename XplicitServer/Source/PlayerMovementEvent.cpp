/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: PlayerMovementEvent.h
 *			Purpose: Player Movement Event handler.
 *
 * =====================================================================
 */

// bug count = 1

 /**
 * @file
 */

#include <ApplicationContext.h>
#include "PlayerMovementEvent.h"

namespace Xplicit
{
	PlayerMovementEvent::PlayerMovementEvent() : mNetwork(nullptr), mThen(IRR->getTimer()->getTime()) {}
	PlayerMovementEvent::~PlayerMovementEvent() = default;

	const char* PlayerMovementEvent::name() noexcept { return ("PlayerMovementEvent"); }

	void PlayerMovementEvent::operator()()
	{
		if (!mNetwork)
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		auto players = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (size_t i = 0; i < players.size(); ++i)
		{
			PlayerComponent* ply = players[i];

			if (ply == nullptr ||
				ply->health() <= 0)
				continue;

			NetworkPeer* peer = ply->get();

			if (peer->public_hash != peer->packet.public_hash)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) // here
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

				/* send server delta to player, so that he is not out of touch. */
				peer->packet.speed[XPLICIT_NETWORK_DELTA] = (IRR->getTimer()->getTime() - mThen) / XPLICIT_DELTA_TIME;

				ply->freeze_for(XPLICIT_MOVEMENT_RATE);
			}
		}
	}
}

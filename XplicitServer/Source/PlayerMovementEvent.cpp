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

#include <Root.h>
#include "PlayerMovementEvent.h"

namespace Xplicit
{
	PlayerMovementEvent::PlayerMovementEvent() 
		: mNetwork(nullptr), mThen(IRR->getTimer()->getTime()) 
	{
		mGameVar = GameVarManager::get_singleton_ptr()->get("Server-DefaultVelocity");
		
		if (!mGameVar)
		{
			mGameVar = Xplicit::GameVarManager::get_singleton_ptr()->create("Server-DefaultVelocity",
				"0.035f",
				Xplicit::GameVarView::FLAG_SERVER_ONLY | Xplicit::GameVarView::FLAG_CHEAT);

		}
	}

	PlayerMovementEvent::~PlayerMovementEvent() = default;

	const char* PlayerMovementEvent::name() noexcept { return ("PlayerMovementEvent"); }

	void PlayerMovementEvent::operator()()
	{
		if (!mNetwork)
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		auto players = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("PlayerComponent");
		float speed = mGameVar->as_float();

		for (std::size_t i = 0; i < players.size(); ++i)
		{
			PlayerComponent* ply = players[i];

			if (ply == nullptr ||
				ply->health() <= 0)
				continue;

			NetworkPeer* peer = ply->get();

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
				continue;

			if (peer->public_hash != peer->packet.public_hash)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) // here
			{
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					ply->pos().Z += speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					ply->pos().Z -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					ply->pos().X -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					ply->pos().X += speed;

				peer->packet.speed[XPLICIT_NETWORK_X] = speed;
				peer->packet.speed[XPLICIT_NETWORK_Y] = speed;
				peer->packet.speed[XPLICIT_NETWORK_Z] = speed;

				peer->packet.public_hash = ply->get()->public_hash;

				/* send server delta to player, so that he is not out of touch. */
				peer->packet.speed[XPLICIT_NETWORK_DELTA] = (IRR->getTimer()->getTime() - mThen) / XPLICIT_DELTA_TIME;

				/* finally accept request */
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

				ply->freeze_for(XPLICIT_MOVEMENT_RATE);

				/* we're done for this one. */
				peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_INVALID;
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_INVALID;
			}
		}
	}
}

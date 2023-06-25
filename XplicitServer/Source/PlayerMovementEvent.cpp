/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
		: 
		mNetwork(nullptr),
		mDelta(0)
	{
		mVelocityVar = GameVarManager::get_singleton_ptr()->get("Server-DefaultVelocity");
		
		if (!mVelocityVar)
		{
			mVelocityVar = Xplicit::GameVarManager::get_singleton_ptr()->create("Server-DefaultVelocity",
				"0.035f",
				Xplicit::GameVar::FLAG_SERVER_ONLY | Xplicit::GameVar::FLAG_CHEAT);

			mDeltaVar = Xplicit::GameVarManager::get_singleton_ptr()->create("Server-Delta",
				"0.01",
				Xplicit::GameVar::FLAG_SERVER_ONLY | Xplicit::GameVar::FLAG_CHEAT);
		}
	}

	PlayerMovementEvent::~PlayerMovementEvent() = default;

	const char* PlayerMovementEvent::name() noexcept { return ("PlayerMovementEvent"); }

	void PlayerMovementEvent::operator()()
	{
		if (!mNetwork)
			mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

		const auto players = ComponentManager::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");
		NetworkFloat speed = mVelocityVar->as_float();
		
		for (std::size_t i = 0; i < players.size(); ++i)
		{
			HumanoidComponent* ply = players[i];

			if (ply == nullptr ||
				ply->health() < 1 ||
				ply->get_peer() == nullptr)
				continue;

			NetworkInstance* peer = ply->get_peer();

			if (peer->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) // here, we check if pos command is set.
			{
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					ply->pos().Z += speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					ply->pos().Z -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					ply->pos().X -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					ply->pos().X += speed;

				peer->packet.pos[XPLICIT_NETWORK_X] = ply->pos().X;
				peer->packet.pos[XPLICIT_NETWORK_Y] = ply->pos().Y;
				peer->packet.pos[XPLICIT_NETWORK_Z] = ply->pos().Z;

				/* send server delta to player, so that he is not out of touch. */
				peer->packet.pos[XPLICIT_NETWORK_DELTA] = mDelta;

				/* finally accept request */
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				peer->packet.public_hash = peer->public_hash;
			}
		}

		mDelta += mDeltaVar->as_float();
	}
}

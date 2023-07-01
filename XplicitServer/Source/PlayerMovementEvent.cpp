/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

// bug count = 0

 /**
 * @file
 */

#include "PlayerMovementEvent.h"

#include <lua/lua.hpp>
#include <Root.h>

namespace Xplicit
{
	PlayerMovementEvent::PlayerMovementEvent() 
		: 
		mNetwork(nullptr),
		mDeltaTime(0UL)
	{
		mVelocityVar = GameVarManager::get_singleton_ptr()->get("Server-Velocity");
		
		if (!mVelocityVar)
		{
			mVelocityVar = Xplicit::GameVarManager::get_singleton_ptr()->create("Server-Velocity",
				"0.035f",
				Xplicit::GameVar::FLAG_SERVER_ONLY | Xplicit::GameVar::FLAG_CHEAT);

		}

		mDeltaVar = GameVarManager::get_singleton_ptr()->get("Server-DeltaTime");

		if (!mDeltaVar)
		{
			mDeltaVar = Xplicit::GameVarManager::get_singleton_ptr()->create("Server-DeltaTime",
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
				ply->get_health() < 1 ||
				ply->get_peer() == nullptr)
				continue;

			NetworkInstance* peer = ply->get_peer();

			if (peer->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) // here, we check if pos command is set.
			{
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					ply->get_attribute().pos().Z += speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					ply->get_attribute().pos().Z -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					ply->get_attribute().pos().X -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					ply->get_attribute().pos().X += speed;

				peer->packet.pos[XPLICIT_NETWORK_X] = ply->get_attribute().pos().X;
				peer->packet.pos[XPLICIT_NETWORK_Y] = ply->get_attribute().pos().Y;
				peer->packet.pos[XPLICIT_NETWORK_Z] = ply->get_attribute().pos().Z;

				/* send server delta to player, so that he is not out of touch. */
				peer->packet.pos[XPLICIT_NETWORK_DELTA] = mDeltaTime;

				/* finally accept request */
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				peer->packet.public_hash = peer->public_hash;

				XPLICIT_INFO("Humanoid:Move [EVENT]");

				if (ply->get_attribute().script() &&
					ply->get_attribute().script()->name() == "Move")
					ply->get_attribute().script()->run();

				XPLICIT_INFO("Engine:Move [EVENT]");

				Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:Move()");
			}
		}

		mDeltaTime += (mDeltaVar->as_float() / 1000.f);
	}
}

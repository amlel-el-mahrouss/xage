/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

// bug count = 0

 /**
 * @file
 */

#include "MovementEvent.h"
#include <CLua/CLua.hpp>

namespace Xplicit
{
	MovementEvent::MovementEvent() 
		: 
		mDeltaTime(0UL)
	{
		mVelocityVar = GameVarManager::get_singleton_ptr()->get("Velocity");
		
		if (!mVelocityVar)
		{
			mVelocityVar = Xplicit::GameVarManager::get_singleton_ptr()->create("Velocity",
				"0.035f",
				Xplicit::GameVar::FLAG_SERVER_ONLY | Xplicit::GameVar::FLAG_CHEAT);

		}

		mDeltaVar = GameVarManager::get_singleton_ptr()->get("DeltaTime");

		if (!mDeltaVar)
		{
			mDeltaVar = Xplicit::GameVarManager::get_singleton_ptr()->create("DeltaTime",
				"0.01",
				Xplicit::GameVar::FLAG_SERVER_ONLY | Xplicit::GameVar::FLAG_CHEAT);
		}
	}

	MovementEvent::~MovementEvent() = default;

	const char* MovementEvent::name() noexcept { return ("MovementEvent"); }

	void MovementEvent::operator()()
	{
		const auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");
		NetworkFloat speed = mVelocityVar->as_float();
		
		for (std::size_t i = 0; i < humanoids.size(); ++i)
		{
			HumanoidComponent* humanoid = humanoids[i];

			if (humanoid == nullptr ||
				humanoid->get_health() < 1 ||
				humanoid->get_peer() == nullptr)
				continue;

			NetworkPeer* peer = humanoid->get_peer();

			if (peer->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			//! No need to edit this piece of code, WorldCollisionEvent will take care of this.
			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) //! Here we check if pos command is set.
			{
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP)
					humanoid->get_attribute().pos().Y += speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					humanoid->get_attribute().pos().Z += speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					humanoid->get_attribute().pos().Z -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					humanoid->get_attribute().pos().X -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					humanoid->get_attribute().pos().X += speed;

				peer->packet.pos[XPLICIT_NETWORK_X] = humanoid->get_attribute().pos().X;
				peer->packet.pos[XPLICIT_NETWORK_Y] = humanoid->get_attribute().pos().Y;
				peer->packet.pos[XPLICIT_NETWORK_Z] = humanoid->get_attribute().pos().Z;

				/* send server delta to player, so that he is not out of touch. */
				peer->packet.pos[XPLICIT_NETWORK_DELTA] = mDeltaTime;

				/* finally accept request */
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				peer->packet.public_hash = peer->public_hash;

#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("World:Move [EVENT]");
#endif // ifdef XPLICIT_DEBUG

				String fmt = std::format("World:Move({}, {}, {})",
					std::to_string(humanoid->get_attribute().pos().X),
					std::to_string(humanoid->get_attribute().pos().Y),
					std::to_string(humanoid->get_attribute().pos().Z));

				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}
		}

		mDeltaTime += (mDeltaVar->as_float() / 1000.f);
	}
}

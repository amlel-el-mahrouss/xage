/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
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
	constexpr const double XPLICIT_DELTA_PER_SECOND = 1000.f;

	MovementEvent::MovementEvent() 
		: 
		mDeltaTime(0UL),
		mDeltaVar(nullptr)
	{
		mDeltaVar = GameVarManager::get_singleton_ptr()->get("DeltaTime");

		if (!mDeltaVar)
		{
			mDeltaVar = Xplicit::GameVarManager::get_singleton_ptr()->create("DeltaTime",
				"0.01",
				Xplicit::GameVar::FLAG_SERVER_ONLY | Xplicit::GameVar::FLAG_CHEAT);
		}
	}

	MovementEvent::~MovementEvent()
	{
		if (mDeltaVar)
			delete mDeltaVar;
	}

	const char* MovementEvent::name() noexcept { return ("MovementEvent"); }

	void MovementEvent::operator()()
	{
		const auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");
		
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
				NetworkFloat speed = humanoid->get_walk_speed();

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP &&
					humanoid->get_attribute().pos().Z < 1)
					humanoid->get_attribute().pos().Z += humanoid->get_jump_power();
				else if (humanoid->get_attribute().pos().Y > 0)
					humanoid->get_attribute().pos().Z -= mDeltaVar->as_float();

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					humanoid->get_attribute().pos().X += speed * mDeltaTime;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					humanoid->get_attribute().pos().X -= speed * mDeltaTime;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					humanoid->get_attribute().pos().Y -= speed * mDeltaTime;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					humanoid->get_attribute().pos().Y += speed * mDeltaTime;

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

				String path("_G.World.Players.");
				path += peer->xplicit_id.as_string();

				humanoid->get_class()->assign("IsMoving", "true");

				String fmt = std::format("World:Move({})", path);
				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

				humanoid->get_class()->assign("IsMoving", "false");
			}
		}

		mDeltaTime += (mDeltaVar->as_float() / XPLICIT_DELTA_PER_SECOND);

		String heartbeat_fmt = std::format("World.Settings.DeltaTime.Value = {}", std::to_string(mDeltaTime));
		Lua::CLuaStateManager::get_singleton_ptr()->run_string(heartbeat_fmt.c_str());
	}
}

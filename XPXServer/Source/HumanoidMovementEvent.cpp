/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

// bug count = 0

 /**
 * @file
 */

#include "HumanoidMovementEvent.h"

namespace XPX
{
	HumanoidMovementEvent::HumanoidMovementEvent() = default;
	HumanoidMovementEvent::~HumanoidMovementEvent() = default;

	const char* HumanoidMovementEvent::name() noexcept { return ("HumanoidMovementEvent"); }

	void HumanoidMovementEvent::operator()()
	{
		const auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>();
		
		for (std::size_t i = 0; i < humanoids.size(); ++i)
		{
			HumanoidComponent* humanoid = humanoids[i];

			if (humanoid == nullptr ||
				!humanoid->can_spawn() ||
				humanoid->get_peer() == nullptr ||
				humanoid->get_peer()->packet.hash != humanoid->get_peer()->hash ||
				humanoid->get_class()->index_as_bool("Anchored"))
				continue;

			NetworkPeer* peer = humanoid->get_peer();

			if (peer->packet.channel & XPLICIT_CHANNEL_CHAT)
				continue;

			//! No need to edit this piece of code, WorldCollisionEvent will take care of this.
			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) //! Here we check if pos command is set.
			{
				NetworkFloat speed = humanoid->get_walk_speed();

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP &&
					humanoid->get_class()->pos().Y < 1)
					humanoid->get_class()->pos().Y = humanoid->get_jump_power();
				
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					humanoid->get_class()->pos().Z += speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					humanoid->get_class()->pos().Z -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					humanoid->get_class()->pos().X -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					humanoid->get_class()->pos().X += speed;

				peer->packet.pos[XPLICIT_NETWORK_X] = speed;
				peer->packet.pos[XPLICIT_NETWORK_Y] = speed;
				peer->packet.pos[XPLICIT_NETWORK_Z] = speed;

				/* finally accept request */

				peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				peer->packet.public_hash = peer->public_hash;

#ifdef XPLICIT_DEBUG
				XPLICIT_INFO("world:Move [EVENT]");
#endif // ifdef XPLICIT_DEBUG

				String path = "world.Players.";
				path += peer->xplicit_id.as_string();

				String fmt = fmt::format("world:Move({})", path);
				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt);
			}
		}
	}
}

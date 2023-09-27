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

#include "CharacterMovementEvent.h"

namespace XPX
{
	CharacterMovementEvent::CharacterMovementEvent() = default;
	CharacterMovementEvent::~CharacterMovementEvent() = default;

	const char* CharacterMovementEvent::name() noexcept { return ("CharacterMovementEvent"); }

	void CharacterMovementEvent::operator()()
	{
		const auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<CharacterComponent>();
		
		for (std::size_t i = 0; i < humanoids.size(); ++i)
		{
			CharacterComponent* humanoid = humanoids[i];

			if (humanoid == nullptr ||
				!humanoid->can_spawn() ||
				humanoid->get_peer() == nullptr ||
				humanoid->get_peer()->packet.hash != humanoid->get_peer()->hash)
				continue;

			NetworkPeer* peer = humanoid->get_peer();

			if (peer->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			//! No need to edit this piece of code, WorldCollisionEvent will take care of this.
			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS) //! Here we check if pos command is set.
			{
				NetworkFloat speed = humanoid->get_walk_speed();

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP)
					humanoid->pos().Y += humanoid->get_jump_power();
		
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					humanoid->pos().Z += speed;
				
				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					humanoid->pos().Z -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					humanoid->pos().X -= speed;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					humanoid->pos().X += speed;
			}
		}
	}
}

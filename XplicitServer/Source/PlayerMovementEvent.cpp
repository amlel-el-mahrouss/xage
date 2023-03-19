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
#include "CommonPackets.h"

namespace Xplicit
{
	/// <summary>
	/// xplicit_can_move: Checks if a player can move
	/// </summary>
	/// <param name="actors">The Actor List</param>
	/// <param name="actor">The Actor Pointer</param>
	/// <returns>True if it can move.</returns>
	static bool xplicit_can_move(ActorArray& actors, Actor* actor)
	{
		XPLICIT_ASSERT(actor);

		if (!actor->can_collide())
			return false;

		if (actors.empty())
			return false;

		for (size_t i = 0; i < actors.size(); ++i)
		{
			if (actors[i]->health() < 1)
				continue;

			if (actors[i] == actor)
				continue;

			if (actors[i]->Position == actor->Position)
			{
				actor->health(0);
				return false;
			}
		}

		return true;
	}

	/// <summary>
	/// The Main loop behind the Actor movement event.
	/// Basic and simple.
	/// </summary>
	void PlayerMovementEvent::operator()()
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<Actor>("Actor");

		for (size_t i = 0; i < actors.size(); ++i)
		{
			Actor* actor = actors[i];

			if (!actor ||
				actor->health() <= 0 ||
				!actor->get())
				continue;

			if (actor->get()->public_hash != actor->get()->public_hash)
				continue;

			NetworkPeer* peer = actor->get();

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD ||
				peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD ||
				peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT ||
				peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
			{
				if (xplicit_can_move(actors, actor))
				{
					auto pos_packet = peer->get_as<PositionPacket>();

					pos_packet->X = actor->Position.X +=
						(peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD ? 10.f : 
							(peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD ? -10.f : 0));

					pos_packet->Y = actor->Position.Y +=
						(peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_FORWARD ? 10.f :
							(peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_BACKWARD ? -10.f : 0));

					peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
					peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;
				}
			}
		}
	}
}
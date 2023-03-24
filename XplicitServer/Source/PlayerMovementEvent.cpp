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
	/// <summary>
	/// xplicit_can_move: Checks if a player can move
	/// </summary>
	/// <param name="actors">Type: ActorArray</param>
	/// <param name="actor">Type: Actor* Description: Actor pointer</param>
	/// <returns>Boolean: Returns true if the actor can move.</returns>
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

	PlayerMovementEvent::PlayerMovementEvent()
		: m_walk_speed_gamevar(GameVarSingleton::get_singleton_ptr()->get("Xplicit::WalkSpeed")), 
		m_side_speed_gamevar(GameVarSingleton::get_singleton_ptr()->get("Xplicit::SideSpeed")), 
		m_jump_height_gamevar(GameVarSingleton::get_singleton_ptr()->get("Xplicit::JumpHeight"))
	{
		auto val = std::to_string(XPLICIT_DEFAULT_VEL);

		// create them if not already created
		if (!m_walk_speed_gamevar)
			m_walk_speed_gamevar= GameVarSingleton::get_singleton_ptr()->create("Xplicit::WalkSpeed", val.c_str(), GameVarView::FLAG_SERVER_ONLY | GameVarView::FLAG_CHEAT);

		if (!m_side_speed_gamevar)
			m_side_speed_gamevar = GameVarSingleton::get_singleton_ptr()->create("Xplicit::SideSpeed", val.c_str(), GameVarView::FLAG_SERVER_ONLY | GameVarView::FLAG_CHEAT);

		if (!m_jump_height_gamevar)
			m_jump_height_gamevar = GameVarSingleton::get_singleton_ptr()->create("Xplicit::JumpHeight", val.c_str(), GameVarView::FLAG_SERVER_ONLY | GameVarView::FLAG_CHEAT);
	}

	PlayerMovementEvent::~PlayerMovementEvent() 
	{
		
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

			NetworkPeer* peer = actor->get();

			if (peer->public_hash != peer->packet.public_hash)
				continue;

			if (xplicit_can_move(actors, actor))
			{
				peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					actor->Position.X += m_walk_speed_gamevar ? m_walk_speed_gamevar->as_float() : XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					actor->Position.X -= m_walk_speed_gamevar ? m_walk_speed_gamevar->as_float() : XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					actor->Position.Y -= m_side_speed_gamevar ? m_side_speed_gamevar->as_float() : XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					actor->Position.Y -= m_side_speed_gamevar ? m_side_speed_gamevar->as_float() : XPLICIT_DEFAULT_VEL;

				if (peer->packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP)
					actor->Position.Z -= m_jump_height_gamevar ? m_jump_height_gamevar->as_float() : XPLICIT_DEFAULT_VEL;
			}
		}
	}
}

#undef XPLICIT_DEFAULT_VEL
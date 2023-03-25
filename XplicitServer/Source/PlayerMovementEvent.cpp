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
	/// <param name="actor">Type: Actor*</param>
	/// <returns>Boolean: Returns true if the actor can move.</returns>
	static bool xplicit_can_move(PlayerArray& actors, PlayerComponent* actor)
	{
		if (!actor)
			return false;

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

			if (actors[i]->get() == actor->get())
				return false;
		}

		return true;
	}

	PlayerMovementEvent::PlayerMovementEvent()
		: m_walk_var(GameVarSingleton::get_singleton_ptr()->get("Xplicit::WalkSpeed")), 
		m_side_var(GameVarSingleton::get_singleton_ptr()->get("Xplicit::SideSpeed")), 
		m_jump_var(GameVarSingleton::get_singleton_ptr()->get("Xplicit::JumpHeight"))
	{
		auto val = std::to_string(XPLICIT_DEFAULT_VEL);

		// create them if not already created
		if (!m_walk_var)
			m_walk_var= GameVarSingleton::get_singleton_ptr()->create("GV_WalkSpeed",
				val.c_str(), 
				GameVarView::FLAG_SERVER_ONLY | GameVarView::FLAG_CHEAT);

		if (!m_side_var)
			m_side_var = GameVarSingleton::get_singleton_ptr()->create("GV_SideSpeed", 
				val.c_str(),
				GameVarView::FLAG_SERVER_ONLY | GameVarView::FLAG_CHEAT);

		if (!m_jump_var)
			m_jump_var = GameVarSingleton::get_singleton_ptr()->create("GV_JumpHeight", 
				val.c_str(), 
				GameVarView::FLAG_SERVER_ONLY | GameVarView::FLAG_CHEAT);
	}

	PlayerMovementEvent::~PlayerMovementEvent() {}

	/// <summary>
	/// The Main loop behind the Actor movement event.
	/// Basic and simple.
	/// </summary>
	void PlayerMovementEvent::operator()()
	{
		auto actors = ComponentManager::get_singleton_ptr()->all_of<PlayerComponent>("Player");

		for (size_t i = 0; i < actors.size(); ++i)
		{
			PlayerComponent* actor = actors[i];

			if (!actor ||
				actor->health() <= 0 ||
				!actor->get())
				continue;

			NetworkPeer* peer = actor->get();

			if (peer->public_hash != peer->packet.public_hash)
				continue;

			if (peer->packet.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] != NETWORK_CMD_ACCEPT &&
				!actor->is_frozen())
			{
				if (xplicit_can_move(actors, actor))
				{
					peer->packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] = NETWORK_CMD_ACCEPT;

					if (peer->packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
						actor->pos().Z += m_walk_var ? m_walk_var->as_float() : XPLICIT_DEFAULT_VEL;

					if (peer->packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
						actor->pos().Z -= m_walk_var ? m_walk_var->as_float() : XPLICIT_DEFAULT_VEL;

					if (peer->packet.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
						actor->pos().X -= m_side_var ? m_side_var->as_float() : XPLICIT_DEFAULT_VEL;

					if (peer->packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
						actor->pos().X += m_side_var ? m_side_var->as_float() : XPLICIT_DEFAULT_VEL;

					if (peer->packet.cmd[XPLICIT_NETWORK_CMD_JUMP] == NETWORK_CMD_JUMP)
						actor->pos().Y += m_jump_var ? m_jump_var->as_float() : XPLICIT_DEFAULT_VEL;

					peer->packet.speed[XPLICIT_NETWORK_X] = m_walk_var ? m_walk_var->as_float() : XPLICIT_DEFAULT_VEL;
					peer->packet.speed[XPLICIT_NETWORK_Y] = m_side_var ? m_side_var->as_float() : XPLICIT_DEFAULT_VEL;
					peer->packet.speed[XPLICIT_NETWORK_Z] = m_jump_var ? m_jump_var->as_float() : XPLICIT_DEFAULT_VEL;

					actor->freeze(XPLICIT_MOVEMENT_RATE);
				}
			}
		}
	}
}

#undef XPLICIT_DEFAULT_VEL
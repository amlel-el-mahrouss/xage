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

#pragma once

#include "SDK.h"
#include "PlayerComponent.h"

namespace Xplicit
{
	/// <summary>
	/// Either accepts of refuses player movement.
	/// Refuses if:
	///		- Player is dead
	///		- Player is inactive
	/// </summary>
	class PlayerMovementEvent : public Event
	{
	public:
		PlayerMovementEvent();
		virtual ~PlayerMovementEvent();

		PlayerMovementEvent& operator=(const PlayerMovementEvent&) = default;
		PlayerMovementEvent(const PlayerMovementEvent&) = default;

		virtual const char* name() noexcept override { return ("PlayerMovementEvent"); }
		virtual void operator()() override;

	private:
		GameVarViewPtr m_jump_height_gamevar;
		GameVarViewPtr m_walk_speed_gamevar;
		GameVarViewPtr m_side_speed_gamevar;

	};
}
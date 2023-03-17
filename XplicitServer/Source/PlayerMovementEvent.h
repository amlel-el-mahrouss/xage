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

#include "Actor.h"

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
		PlayerMovementEvent() = default;
		virtual ~PlayerMovementEvent() = default;

		PlayerMovementEvent& operator=(const PlayerMovementEvent&) = default;
		PlayerMovementEvent(const PlayerMovementEvent&) = default;

		virtual const char* name() noexcept override { return ("PlayerMovementEvent"); }
		virtual void operator()() override;

	};
}
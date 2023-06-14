/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: PlayerMovementEvent.h
 *			Purpose: Player Movement Event
 *
 * =====================================================================
 */

#pragma once

#include "CommonInc.h"
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

	public:
		virtual const char* name() noexcept override;
		virtual void operator()() override;

	private:
		NetworkServerComponent* mNetwork;
		float mThen;

	};
}
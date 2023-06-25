/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"
#include "HumanoidComponent.h"

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
		~PlayerMovementEvent() override;

		PlayerMovementEvent& operator=(const PlayerMovementEvent&) = default;
		PlayerMovementEvent(const PlayerMovementEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;

	private:
		NetworkServerComponent* mNetwork;
		GameVarPtr mVelocityVar;
		GameVarPtr mDeltaVar;
		float mDelta;

	};
}
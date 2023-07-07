/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Nplicit.h>
#include <RigidBody.h>

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
		explicit PlayerMovementEvent();
		~PlayerMovementEvent() override;

	public:
		PlayerMovementEvent& operator=(const PlayerMovementEvent&) = default;
		PlayerMovementEvent(const PlayerMovementEvent&) = default;
		
	public:
		const char* name() noexcept override;
		void operator()() override;

	private:
		GameVarPtr mVelocityVar;
		GameVarPtr mDeltaVar;

	private:
		float mDeltaTime;

	};
}
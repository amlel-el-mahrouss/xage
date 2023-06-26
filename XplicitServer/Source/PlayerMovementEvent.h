/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
		PlayerMovementEvent();
		~PlayerMovementEvent() override;

		PlayerMovementEvent& operator=(const PlayerMovementEvent&) = default;
		PlayerMovementEvent(const PlayerMovementEvent&) = default;
		
		const char* name() noexcept override;
		void operator()() override;

		std::shared_ptr<SolverSystem<float>> get_solver() noexcept;

	private:
		std::shared_ptr<SolverSystem<float>> mSolver;

	private:
		NetworkServerComponent* mNetwork;
		GameVarPtr mVelocityVar;
		GameVarPtr mDeltaVar;
		float mDeltaTime;

	};
}
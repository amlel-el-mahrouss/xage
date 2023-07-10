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
	
	class MovementEvent : public Event
	{
	public:
		explicit MovementEvent();
		~MovementEvent() override;

	public:
		MovementEvent& operator=(const MovementEvent&) = default;
		MovementEvent(const MovementEvent&) = default;
		
	public:
		const char* name() noexcept override;
		void operator()() override;

	private:
		GameVarPtr mDeltaVar;
		double mDeltaTime;

	};
}
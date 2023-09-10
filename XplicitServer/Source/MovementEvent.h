/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "HumanoidComponent.h"

#include <Nplicit.h>
#include <RigidBody.h>

namespace XPX
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
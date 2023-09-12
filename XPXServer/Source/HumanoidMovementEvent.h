/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "HumanoidComponent.h"

// NplicitNgin Rigid bodies include.
#include <RigidBody.h>

namespace XPX
{
	/// <summary>
	/// Either accepts of refuses player movement.
	/// Refuses if:
	///		- Player is dead
	///		- Player is nil (disconnected player)
	/// </summary>
	
	class HumanoidMovementEvent : public Event
	{
	public:
		HumanoidMovementEvent();
		~HumanoidMovementEvent() override;

	public:
		HumanoidMovementEvent& operator=(const HumanoidMovementEvent&) = default;
		HumanoidMovementEvent(const HumanoidMovementEvent&) = default;
		
	public:
		const char* name() noexcept override;
		void operator()() override;

	private:
		GameVarPtr mDeltaVar;
		double mDeltaTime;

	};
}
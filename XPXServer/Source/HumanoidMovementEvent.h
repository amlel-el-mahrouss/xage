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
		XPLICIT_COPY_DEFAULT(HumanoidMovementEvent);

	public:
		const char* name() noexcept override;
		void operator()() override;

	};
}
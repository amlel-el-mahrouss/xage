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
	/// This class takes care of player movements.
	/// Only works if the player is alive.
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
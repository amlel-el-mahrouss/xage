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
#include "CharacterComponent.h"

namespace XPX
{
	/// <summary>
	/// This class takes care of player movements.
	/// Only works if the player is alive.
	/// </summary>
	class CharacterMovementEvent : public Event
	{
	public:
		CharacterMovementEvent();
		~CharacterMovementEvent() override;

	public:
		XPLICIT_COPY_DEFAULT(CharacterMovementEvent);

	public:
		const char* name() noexcept override;
		void operator()() override;

	};
}
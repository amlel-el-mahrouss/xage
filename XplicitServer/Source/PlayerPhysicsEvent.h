/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Movements.
 *
 * =====================================================================
 */

#pragma once

#include "Actor.h"

namespace Xplicit
{
	class PlayerPhysicsEvent : public Event
	{
	public:
		PlayerPhysicsEvent() = default;
		virtual ~PlayerPhysicsEvent() = default;

		PlayerPhysicsEvent& operator=(const PlayerPhysicsEvent&) = default;
		PlayerPhysicsEvent(const PlayerPhysicsEvent&) = default;

		virtual const char* name() noexcept override { return ("PlayerPhysicsEvent"); }
		virtual void operator()() override;

	};
}
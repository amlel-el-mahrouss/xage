/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerSpawnDeathEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "Actor.h"

namespace Xplicit
{
	class PlayerSpawnDeathEvent : public Event
	{
	public:
		PlayerSpawnDeathEvent() = default;
		virtual ~PlayerSpawnDeathEvent() = default;

		PlayerSpawnDeathEvent& operator=(const PlayerSpawnDeathEvent&) = default;
		PlayerSpawnDeathEvent(const PlayerSpawnDeathEvent&) = default;

		virtual const char* name() noexcept override { return ("PlayerSpawnDeathEvent"); }

		virtual void operator()() override;

	private:
		std::vector<Actor*> m_dead_actors;

	};
}
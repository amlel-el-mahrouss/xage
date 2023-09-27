/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: CharacterHealthMonitorEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "SpawnComponent.h"
#include "CharacterComponent.h"

namespace XPX
{
	class CharacterHealthMonitorEvent : public Event
	{
	public:
		CharacterHealthMonitorEvent() 
			: mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
			  mSpawner(ComponentSystem::get_singleton_ptr()->get<SpawnComponent>("SpawnComponent"))
		{
			XPLICIT_ASSERT(mNetwork);
		}

		virtual ~CharacterHealthMonitorEvent() = default;

		CharacterHealthMonitorEvent& operator=(const CharacterHealthMonitorEvent&) = default;
		CharacterHealthMonitorEvent(const CharacterHealthMonitorEvent&) = default;

		virtual const char* name() noexcept override { return ("CharacterHealthMonitorEvent"); }

		virtual void operator()() override;

	private:
		NetworkServerComponent* mNetwork;
		SpawnComponent* mSpawner;

	};
}
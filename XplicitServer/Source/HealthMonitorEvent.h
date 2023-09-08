/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: HealthMonitorEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "SpawnComponent.h"
#include "HumanoidComponent.h"

namespace Xplicit
{
	class HealthMonitorEvent : public Event
	{
	public:
		HealthMonitorEvent() 
			: mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
			  mSpawner(ComponentSystem::get_singleton_ptr()->get<SpawnComponent>("SpawnComponent"))
		{
			XPLICIT_ASSERT(mNetwork);
		}

		virtual ~HealthMonitorEvent() = default;

		HealthMonitorEvent& operator=(const HealthMonitorEvent&) = default;
		HealthMonitorEvent(const HealthMonitorEvent&) = default;

		virtual const char* name() noexcept override { return ("HealthMonitorEvent"); }

		virtual void operator()() override;

	private:
		NetworkServerComponent* mNetwork;
		SpawnComponent* mSpawner;

	};
}
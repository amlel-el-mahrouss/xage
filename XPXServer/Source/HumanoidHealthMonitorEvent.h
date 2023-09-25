/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: HumanoidHealthMonitorEvent.h
 *			Purpose: Spawn and Death handling
 *
 * =====================================================================
 */

#pragma once

#include "SpawnComponent.h"
#include "HumanoidComponent.h"

namespace XPX
{
	class HumanoidHealthMonitorEvent : public Event
	{
	public:
		HumanoidHealthMonitorEvent() 
			: mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
			  mSpawner(ComponentSystem::get_singleton_ptr()->get<SpawnComponent>("SpawnComponent"))
		{
			XPLICIT_ASSERT(mNetwork);
		}

		virtual ~HumanoidHealthMonitorEvent() = default;

		HumanoidHealthMonitorEvent& operator=(const HumanoidHealthMonitorEvent&) = default;
		HumanoidHealthMonitorEvent(const HumanoidHealthMonitorEvent&) = default;

		virtual const char* name() noexcept override { return ("HumanoidHealthMonitorEvent"); }

		virtual void operator()() override;

	private:
		NetworkServerComponent* mNetwork;
		SpawnComponent* mSpawner;

	};
}
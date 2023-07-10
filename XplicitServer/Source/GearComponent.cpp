/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

// The component itself 
#include "GearComponent.h"

//! This file handles Gears (Sword, guns, tools...)

namespace Xplicit
{
	GearComponent::GearComponent(
		const char* name,
		const char* parent) noexcept
		:
		ClassComponent(Vector<float>(0, 0, 0), 
			Vector<float>(0, 0, 0),
			Color<float>(0, 0, 0),
			nullptr,
			parent,
			name)
	{
		this->insert("Slot", "0");

		this->insert("Enabled", "false");
		this->insert("CanDrop", "false");

		this->insert("Drop", "false");
		this->insert("Tooltip", "'Gear Component'");

		this->insert("LookAt", "{ 0, 0, 0 }");

		this->insert("Click", "function() end");
		this->insert("Activate", "function() end");
		this->insert("Deactivate", "function() end");
	}

	const char* GearComponent::name() noexcept { return "GearComponent"; }

	COMPONENT_TYPE GearComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE GearComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	GearComponent::~GearComponent() = default;

	bool GearComponent::should_update() noexcept { return true; }

	void GearComponent::update(ClassPtr _self) { ClassComponent::update(_self); }

	void GearComponent::click() noexcept
	{
		this->call("Click");
	}

	void GearComponent::activate() noexcept
	{
		this->call("Activate");
	}

	void GearComponent::deactivate() noexcept
	{
		this->call("Deactivate");
	}
}

#undef XPLICIT_LUA_GLOBAL
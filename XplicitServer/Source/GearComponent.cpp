/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

// Include the component class.

#include "ServerReplicationManager.h"
#include "GearComponent.h"

//! This file handles Gears (Sword, Pistol, Build Tools...)

namespace Xplicit
{
	static const String XPLICIT_GEAR_DESTROY(const String& name) noexcept
	{
		String func_proto = "function(self) World.GearService.Destroy(";

		func_proto += "\"";
		func_proto += name;
		func_proto += "\"";
		func_proto += "); end";

		return func_proto;
	}

	GearComponent::GearComponent(
		const char* name,
		const char* parent) noexcept
		:
		ClassComponent(Vector<float>(0, 0, 0), 
			Vector<float>(0, 0, 0),
			Color<float>(0, 0, 0),
			nullptr,
			parent,
			name),
		mParent(parent),
		mName(name)
	{
		this->insert("Owner", parent);
		this->insert("Slot", "0");

		this->insert("Enabled", "false");
		this->insert("CanDrop", "false");

		this->insert("Drop", "false");
		this->insert("Name", std::format("'{}'", name).c_str());

		this->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");

		this->insert("Destroy", XPLICIT_GEAR_DESTROY(name).c_str());
	}

	const char* GearComponent::name() noexcept { return mName.c_str(); }

	COMPONENT_TYPE GearComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE GearComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	GearComponent::~GearComponent() = default;

	bool GearComponent::should_update() noexcept { return true; }

	void GearComponent::update(ClassPtr _self) 
	{ 
		ClassComponent::update(_self);

		GearComponent* self = (GearComponent*)_self;

		if (!self->index_as_bool("Enabled"))
			return;

		self->assign("LookAt.X", "Parent.LookAt.X or 0");
		self->assign("LookAt.Y", "Parent.LookAt.Y or 0");
		self->assign("LookAt.Z", "Parent.LookAt.Z or 0");
	}
}

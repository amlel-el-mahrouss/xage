/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

// Include the component class.

#include "GearComponent.h"
#include "HumanoidComponent.h"

//! This file handles Gears (Sword, Pistol, Build Tools...)

namespace XPX
{
	static const String destroy_gear_class_snippet(const String& name) noexcept
	{
		String func_proto = fmt::format("function(self) XPXDestroyGear(\"{}\"); end",
			name);

		return func_proto;
	}

	GearComponent::GearComponent(const char* name, const char* parent) noexcept
		:
		ClassComponent(Vector<float>(0, 0, 0),
			Vector<float>(0, 0, 0),
			Color<float>(0, 0, 0),
			nullptr,
			parent,
			name),
		mParent(parent),
		mName(name),
		mOwner(nullptr)
	{
		this->insert("Slot", "0");
		this->insert("Equipped", "false");
		this->insert("CanDrop", "false");
		this->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");
		this->assign("Destroy", destroy_gear_class_snippet(name).c_str());
		this->insert("__gc", destroy_gear_class_snippet(name).c_str());
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

		if (self->index_as_bool("CanDrop") &&
			self->get_owner())
		{
			self->assign("Parent", "world");

			self->get_owner()->get_class()->assign(self->name(), "nil");
			self->set_owner(nullptr);
		}

		if (!self->index_as_bool("Equipped"))
			return;

		if (self->get_owner())
		{
			self->pos() = self->get_owner()->get_attribute().pos();
		
			if (self->get_owner()->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_TARGET] == NETWORK_CMD_TARGET)
			{
				self->assign("LookAt.X", std::to_string(self->get_owner()->get_peer()->packet.pos[XPLICIT_NETWORK_X]).c_str());
				self->assign("LookAt.Y", std::to_string(self->get_owner()->get_peer()->packet.pos[XPLICIT_NETWORK_Y]).c_str());
				self->assign("LookAt.Z", std::to_string(self->get_owner()->get_peer()->packet.pos[XPLICIT_NETWORK_Z]).c_str());
			}
		}
	}

	HumanoidComponent* GearComponent::get_owner() noexcept { return mOwner; }
	void GearComponent::set_owner(HumanoidComponent* owner) noexcept { mOwner = owner; }
}

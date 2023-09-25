/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

// Include the component class.

#include "WeaponComponent.h"
#include "HumanoidComponent.h"

//! This file handles Gears (Sword, Pistol, Build Tools...)

namespace XPX
{
	static const String destroy_gear_class_snippet(const String& name) noexcept
	{
		String func_proto = fmt::format("function(self) destroyGear(\"{}\"); end",
			name);

		return func_proto;
	}

	WeaponComponent::WeaponComponent(const char* name, const char* parent) noexcept
		:
		ClassComponent(Vector<NetworkFloat>(0, 0, 0),
			Vector<NetworkFloat>(0, 0, 0),
			Color<NetworkFloat>(0, 0, 0),
			nullptr,
			parent,
			name),
		mParent(parent),
		mName(name),
		mOwner(nullptr)
	{
		this->insert("Slot", "0");
		this->insert("Drop", "false");
		this->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");
		this->assign("Destroy", destroy_gear_class_snippet(name).c_str());
	}

	const char* WeaponComponent::name() noexcept { return mName.c_str(); }

	COMPONENT_TYPE WeaponComponent::type() noexcept { return COMPONENT_LOGIC; }

	PHYSICS_TYPE WeaponComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	WeaponComponent::~WeaponComponent() = default;

	bool WeaponComponent::should_update() noexcept { return true; }

	void WeaponComponent::update(ClassPtr this_ptr) 
	{ 
		if (!this_ptr)
			return;

		ClassComponent::update(this_ptr);
		WeaponComponent* self = (WeaponComponent*)this_ptr;

		if (self->index_as_bool("Drop") &&
			self->get_owner())
		{
			self->assign("Parent", XPLICIT_LUA_NAMESPACE);

			self->get_owner()->get_class()->assign(self->name(), "nil");
			self->set_owner(nullptr);
		}

		if (self->get_owner() &&
			self->get_owner()->get_peer())
		{
			self->pos() = self->get_owner()->get_class()->pos();
		
			if (self->get_owner()->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_TARGET] == NETWORK_CMD_TARGET)
			{
				self->assign("LookAt.X", std::to_string(self->get_owner()->get_peer()->packet.pos[0][XPLICIT_NETWORK_X]));
				self->assign("LookAt.Y", std::to_string(self->get_owner()->get_peer()->packet.pos[0][XPLICIT_NETWORK_Y]));
				self->assign("LookAt.Z", std::to_string(self->get_owner()->get_peer()->packet.pos[0][XPLICIT_NETWORK_Z]));

				self->call_method("Update('LookAt')");
			}
		}
	}

	HumanoidComponent* WeaponComponent::get_owner() noexcept { return mOwner; }
	void WeaponComponent::set_owner(HumanoidComponent* owner) noexcept { mOwner = owner; }
}

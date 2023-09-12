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
		this->insert("Equipped", "false");
		this->insert("CanDrop", "false");
		this->insert("Destroy", destroy_gear_class_snippet(name).c_str());
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
			self->assign("Parent", "World");

			self->get_owner()->get_class()->assign(self->name(), "nil");
			self->set_owner(nullptr);
		}

		if (!self->index_as_bool("Equipped"))
			return;

		if (self->get_owner())
			self->pos() = self->get_owner()->get_attribute().pos();
	}

	HumanoidComponent* GearComponent::get_owner() noexcept { return mOwner; }
	void GearComponent::set_owner(HumanoidComponent* owner) noexcept { mOwner = owner; }
}

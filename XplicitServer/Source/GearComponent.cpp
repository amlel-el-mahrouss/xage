/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

// Include the component class.

#include "GearComponent.h"
#include "HumanoidComponent.h"

//! This file handles Gears (Sword, Pistol, Build Tools...)

namespace Xplicit
{
	static const String destroy_gear_class_snippet(const String& name) noexcept
	{
		String func_proto = std::format("function(self) XPXDestroyGear(\"{}\"); end",
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
		this->insert("LookAt", "{ X = 0, Y = 0, Z = 0 }");
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
			self->assign("Parent", "Parent.Parent");
			self->get_owner()->get_class()->assign(self->name(), "nil");

			for (size_t i = 0; i < self->get_owner()->get_gears().size(); ++i)
			{
				auto& gear = self->get_owner()->get_gears()[i];

				if (gear == self)
				{
					gear = nullptr;
					break;
				}
			}

			self->set_owner(nullptr);
		}

		if (!self->index_as_bool("Equipped"))
			return;

		self->assign("LookAt.X", "Parent.LookAt.X or nil");
		self->assign("LookAt.Y", "Parent.LookAt.Y or nil");
		self->assign("LookAt.Z", "Parent.LookAt.Z or nil");
	}
}

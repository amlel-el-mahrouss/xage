/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include "HumanoidComponent.h"

#include <CLua.hpp>

namespace XPX
{
	SpawnComponent::SpawnComponent(const Vector<NetworkFloat>& vec)
		: ClassComponent("world", "SpawnLocation")
	{
		this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");

		this->mAttribute.pos() = vec;

		auto all_of_humans = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");

		for (auto& human : all_of_humans)
		{
			if (human)
				human->get_attribute().pos() = this->mAttribute.pos();
		}
	}

	SpawnComponent::~SpawnComponent() = default;

	Vector<NetworkFloat>& SpawnComponent::get() noexcept { return mAttribute.pos(); }

	void SpawnComponent::update(ClassPtr class_ptr) 
	{
		ClassComponent::update(class_ptr);

		SpawnComponent* self = (SpawnComponent*)class_ptr;

		if (self)
		{
			String pos_fmt = "{ X = " + std::to_string(self->mAttribute.pos().X) + ", Y = " +
				std::to_string(self->mAttribute.pos().Y) + ", Z = " +
				std::to_string(self->mAttribute.pos().Z) + "," + "}";

			self->assign("Position", pos_fmt);
		}
	}

	COMPONENT_TYPE SpawnComponent::type() noexcept { return (COMPONENT_LOGIC); }
	const char* SpawnComponent::name() noexcept { return ("SpawnComponent"); }

	XPXAttribute& SpawnComponent::get_attribute() noexcept { return mAttribute; }
}
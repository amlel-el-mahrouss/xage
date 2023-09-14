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

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "world."

namespace XPX
{
	SpawnComponent::SpawnComponent(const Vector<NetworkFloat>& vec)
		: Component(), mClass(std::make_unique<Lua::CLuaClass>("SpawnLocation"))
	{
		XPLICIT_ASSERT(mClass);
		mClass->insert("Position", "{ X = 0, Y = 0, Z = 0 }");

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
		SpawnComponent* self = (SpawnComponent*)class_ptr;

		static auto str = "{ X = " + std::to_string(self->mAttribute.pos().X) + ", Y = " +
			std::to_string(self->mAttribute.pos().Y) + ", Z = " +
			std::to_string(self->mAttribute.pos().Z) + "," + "}";

		self->mClass->assign("Position", str.c_str());
	}

	COMPONENT_TYPE SpawnComponent::type() noexcept { return (COMPONENT_LOGIC); }
	const char* SpawnComponent::name() noexcept { return ("SpawnComponent"); }

	XPXAttribute& SpawnComponent::get_attribute() noexcept { return mAttribute; }
}
/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include <CLua/CLua.hpp>

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "World."

namespace XPX
{
	SpawnComponent::SpawnComponent(const Vector<float>& vec) 
		: Component(), mClass(std::make_unique<Lua::CLuaClass>("SpawnLocation"))
	{
		XPLICIT_ASSERT(mClass);
		mClass->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
	}

	SpawnComponent::~SpawnComponent() = default;

	Vector<float>& SpawnComponent::get() noexcept { return mAttribute.pos(); }

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
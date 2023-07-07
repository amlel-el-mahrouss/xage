/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include <lua/CLua.hpp>

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game."

namespace Xplicit
{
	SpawnComponent::SpawnComponent(const Vector<float>& vec) 
		: Component(), mClass(std::make_unique<Lua::CLuaClass>("SpawnLocation"))
	{
		XPLICIT_ASSERT(mClass);
		mClass->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
	}

	SpawnComponent::~SpawnComponent() = default;

	Vector<float>& SpawnComponent::get() noexcept { return mAttribute.pos(); }

	bool SpawnComponent::should_update() noexcept { return true; }

	void SpawnComponent::update() 
	{
		auto str = "{" + std::to_string(mAttribute.pos().X) + "," +
			std::to_string(mAttribute.pos().Y) + "," +
			std::to_string(mAttribute.pos().Z) + "," + "}";

		mClass->assign("Position", str.c_str());
	}

	COMPONENT_TYPE SpawnComponent::type() noexcept { return (COMPONENT_LOGIC); }
	const char* SpawnComponent::name() noexcept { return ("SpawnComponent"); }

	XAttribute& SpawnComponent::get_attribute() noexcept { return mAttribute; }
}
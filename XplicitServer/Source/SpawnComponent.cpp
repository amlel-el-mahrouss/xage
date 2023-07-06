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
		: Component()
	{
		String fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += "Spawn";
		fmt += "= {};";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	SpawnComponent::~SpawnComponent()
	{
		String fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += "Spawn";
		fmt += "= nil;";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	Vector<float>& SpawnComponent::get() noexcept { return mAttribute.pos(); }

	bool SpawnComponent::should_update() noexcept { return true; }

	void SpawnComponent::update() 
	{
		static const char* pos[] = { "X", "Y", "Z" };
		const float pos_raw[] = { mAttribute.pos().X, mAttribute.pos().Y, mAttribute.pos().Z };

		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += "Spawn";
			fmt += ".Position.";
			fmt += pos[i];
			fmt += "= " + std::to_string(pos_raw[i]) + ";";

			Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
	}

	COMPONENT_TYPE SpawnComponent::type() noexcept { return (COMPONENT_LOGIC); }
	const char* SpawnComponent::name() noexcept { return ("SpawnComponent"); }

	XAttribute& SpawnComponent::get_attribute() noexcept { return mAttribute; }
}
/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include <lua/lua.hpp>


#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game."

namespace Xplicit
{
	SpawnComponent::SpawnComponent(const Quaternion<float>& vec) 
		: Component(), mOrigin(vec) 
	{
		String fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += "Spawn";
		fmt += "= {};";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	SpawnComponent::~SpawnComponent()
	{
		String fmt = XPLICIT_LUA_GLOBAL;

		fmt += XPLICIT_LUA_NAMESPACE;
		fmt += "Spawn";
		fmt += "= nil;";

		Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	Quaternion<float>& SpawnComponent::get() noexcept { return mOrigin; }

	bool SpawnComponent::should_update() noexcept { return true; }

	void SpawnComponent::update() 
	{
		static const char* pos[] = { "X", "Y", "Z" };
		const float pos_raw[] = { mOrigin.X, mOrigin.Y, mOrigin.Z };

		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += "Spawn";
			fmt += ".Position.";
			fmt += pos[i];
			fmt += "= " + std::to_string(pos_raw[i]) + ";";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
	}
}
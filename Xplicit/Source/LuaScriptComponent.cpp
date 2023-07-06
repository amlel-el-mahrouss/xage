/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"

namespace Xplicit
{
	void LuaScriptComponent::run() noexcept
	{
		if (!this->mName.empty())
		{
			if (Lua::CLuaStateManager::get_singleton_ptr()->run(this->mName.c_str()) != 0)
			{
				String lua_error_str = "[LuaScriptComponent] error at file: ";

				lua_error_str += this->mName;
				lua_error_str += ", ";
				lua_error_str += lua_tostring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
					-1);

				XPLICIT_INFO(lua_error_str);
			}
		}
	}

	void LuaScriptComponent::update() noexcept {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::name() noexcept { return mName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}
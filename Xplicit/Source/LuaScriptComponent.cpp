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
		Thread job([](String _file) {
			if (!_file.empty())
			{
				if (Lua::XLuaStateManager::get_singleton_ptr()->run(_file.c_str()) != 0)
				{
					String lua_error_str = "[LuaScriptComponent] error at file: ";
					
					lua_error_str += _file;
					lua_error_str += ", ";
					lua_error_str += lua_tostring(Lua::XLuaStateManager::get_singleton_ptr()->state(),
						-1);

					XPLICIT_INFO(lua_error_str);
				}
			}
			}, mName);

		job.detach();
	}

	void LuaScriptComponent::update() noexcept {}

	bool LuaScriptComponent::should_update() noexcept { return false; }

	const char* LuaScriptComponent::name() noexcept { return mEventName.c_str(); }

	COMPONENT_TYPE LuaScriptComponent::type() noexcept { return COMPONENT_SCRIPT; }
}
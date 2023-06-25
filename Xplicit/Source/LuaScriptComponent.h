/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Component.h"
#include <lua/lua.hpp>

namespace Xplicit::Lua
{
	/*
	 * @brief Lua Script Instance.
	 * NOTE: only one LuaScriptComponent should be created. you don't need a lot of them.
	 */

	class XPLICIT_API LuaScriptComponent : public Component
	{
	public:
		LuaScriptComponent() = default;
		~LuaScriptComponent() override = default;

	public:
		XPLICIT_COPY_DEFAULT(LuaScriptComponent);

	public:
		const char* name() noexcept override { return "LuaScriptComponent"; }
		COMPONENT_TYPE type() noexcept override { COMPONENT_SCRIPT; }

	public:
		void run(const char* file)
		{
			if (file)
			{
				if (XLuaStateManager::get_singleton_ptr()->run(file) != 0)
				{
					String lua_error_str = "[LUA] ";
					lua_error_str += lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1);

					XPLICIT_INFO(lua_error_str);

				}
			}
		}

		void do_string(const char* file)
		{
			if (file)
			{
				if (XLuaStateManager::get_singleton_ptr()->run_string(file) != 0)
				{
					String lua_error_str = "[LUA] ";
					lua_error_str += lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1);

					XPLICIT_INFO(lua_error_str);

				}
			}
		}

	};
}
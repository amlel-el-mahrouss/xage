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
		void do_file(const char* file) noexcept
		{
			Thread job([](String _file) {
				if (!_file.empty())
				{
					if (XLuaStateManager::get_singleton_ptr()->run(_file.c_str()) != 0)
					{
						String lua_error_str = "[DoFile (LUA)] ";
						lua_error_str += lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1);

						XPLICIT_INFO(lua_error_str);

					}
				}
			}, file);

			job.detach();
		}

		void do_string(const char* file) noexcept
		{
			Thread job([](String _file) {
				if (!_file.empty())
				{
					if (XLuaStateManager::get_singleton_ptr()->run(_file.c_str()) != 0)
					{
						String lua_error_str = "[RunString (LUA)] ";
						lua_error_str += lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1);

						XPLICIT_INFO(lua_error_str);

					}
				}
			}, file);

			job.detach();
		}

	};
}
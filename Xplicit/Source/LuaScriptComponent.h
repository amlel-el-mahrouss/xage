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

	class XPLICIT_API LuaScriptComponent final : public Component
	{
	public:
		LuaScriptComponent() = delete;

	public:
		explicit LuaScriptComponent(const char* name) : Component(), mName(name) {}
		~LuaScriptComponent() override = default;

	public:
		XPLICIT_COPY_DEFAULT(LuaScriptComponent);

	public:
		const char* name() noexcept override { return "LuaScriptComponent"; }
		COMPONENT_TYPE type() noexcept override { COMPONENT_SCRIPT; }

	public:
		void operator()() noexcept
		{
			Thread job([](String _file) {
				if (!_file.empty())
				{
					if (XLuaStateManager::get_singleton_ptr()->run(_file.c_str()) != 0)
					{
						String lua_error_str = "[LuaScriptComponent] error at file: ";
						lua_error_str += _file;

						XPLICIT_INFO(lua_error_str);
					}
				}
			}, mName);

			job.detach();
		}

	private:
		String mName;

	};
}
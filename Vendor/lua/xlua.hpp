/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

/* Author: Amlal El Mahrouss */
/* XLua for the XplicitNgine. */

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

#define XPLICIT_LUA_NAME "XplicitNgine Lua"
#define XPLICIT_LUA_DESCRIPTION "Custom dialect for Xplicit"

namespace Xplicit::Lua
{
	class XPLICIT_API ILuaStateManager final
	{
	private:
		explicit ILuaStateManager()
			: mL(luaL_newstate())
		{
			XPLICIT_ASSERT(mL);
			luaL_openlibs(mL);
		}

	private:
		lua_State* mL;

	public:
		~ILuaStateManager() = default;

		XPLICIT_COPY_DEFAULT(ILuaStateManager);

	public:
		static ILuaStateManager* get_singleton_ptr() noexcept
		{
			static ILuaStateManager* state = nullptr;

			if (state == nullptr)
				state = new ILuaStateManager();

			return state;
		}

		std::int32_t run(const char* file) noexcept
		{
			if (file)
				return luaL_dofile(mL, file);
			
			return -1;
		}

		std::int32_t run_string(const char* file) noexcept
		{
			if (file)
				return luaL_dostring(mL, file);

			return -1;
		}

	};
}
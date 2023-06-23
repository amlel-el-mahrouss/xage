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
/* XLua for C++ classes. */

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

#define XPLICIT_LUA_NAME "XplicitLua"
#define XPLICIT_LUA_DESCRIPTION "Custom dialect for JACK Engine."

namespace Xplicit::Lua
{
	class ILuaStateManager final
	{
	private:
		explicit ILuaStateManager()
			: mL(lua_open())
		{}

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

		std::int32_t run(const char* file)
		{
			if (file)
				luaL_dofile(file);
		}
		
	};
}
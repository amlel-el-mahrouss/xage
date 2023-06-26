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

#define XPLICIT_LUA_NAME "XLua"
#define XPLICIT_LUA_DESCRIPTION "Custom dialect for Xplicit"

namespace Xplicit::Lua
{
	typedef lua_State* XLuaStatePtr;

	class XPLICIT_API XLuaStateManager final
	{
	private:
		explicit XLuaStateManager()
			: mL(luaL_newstate())
		{
			XPLICIT_ASSERT(mL);
			luaL_openlibs(mL);
		}

	private:
		lua_State* mL;

	public:
		~XLuaStateManager() = default;

		XPLICIT_COPY_DEFAULT(XLuaStateManager);

	public:
		static XLuaStateManager* get_singleton_ptr() noexcept
		{
			static XLuaStateManager* state = nullptr;

			if (state == nullptr)
				state = new XLuaStateManager();

			return state;
		}

		std::int32_t run(const char* file) noexcept
		{
			if (file)
			{
				const auto ret = luaL_dofile(mL, file);

				if (ret > 0)
					std::cout << lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1) << "\n";

				return ret;
			}
			
			return -1;
		}

		std::int32_t run_string(const char* file) noexcept
		{
			if (file)
			{
				const auto ret = luaL_dostring(mL, file);

				if (ret > 0)
					std::cout << lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1) << "\n";

				return ret;
			}

			return -1;
		}

		lua_State* state() noexcept 
		{
			return mL;
		}

	};
}
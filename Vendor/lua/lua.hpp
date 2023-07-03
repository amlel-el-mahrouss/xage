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

#ifndef __XPLICIT_H__
#	include <Xplicit.h>
#endif // ifndef __XPLICIT_H__

#define XPLICIT_LUA_NAME "CLua"
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

			luaopen_base(mL);
			luaopen_math(mL);
			luaopen_string(mL);
			luaopen_table(mL);
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

#ifdef XPLICIT_DEBUG

				if (ret > 0)
					std::cout << lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1) << "\n";

#endif // ifdef XPLICIT_DEBUG

				return ret;
			}
			
			return -1;
		}

		std::int32_t run_string(const char* str) noexcept
		{
			if (str)
			{
				const auto ret = luaL_dostring(mL, str);

#ifdef XPLICIT_DEBUG

				if (ret > 0)
					std::cout << lua_tostring(XLuaStateManager::get_singleton_ptr()->state(), -1) << "\n";

#endif // ifdef XPLICIT_DEBUG

				return ret;
			}

			return -1;
		}

		lua_State* state() noexcept 
		{
			return mL;
		}

	};

	// WiP

	inline bool XLuaCreateClass(const char* name)
	{
		return Lua::XLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}", name).c_str());
	}

	inline bool XLuaAddMethod(const char* klass, const char* symbol, const char* reference)
	{
		return Lua::XLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}.{} = {}", klass, symbol, reference).c_str());
	}
}
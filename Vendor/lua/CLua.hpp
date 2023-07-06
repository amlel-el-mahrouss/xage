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
/* CLua for the XplicitNgine. */

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

#ifndef __XPLICIT_H__
#	include <Xplicit.h>
#endif // ifndef __XPLICIT_H__

#define XPLICIT_LUA_NAME "CLua"
#define XPLICIT_LUA_DESCRIPTION "Custom lua dialect for Xplicit"

namespace Xplicit::Lua
{
	typedef lua_State* CLuaStatePtr;

	class XPLICIT_API CLuaStateManager final
	{
	private:
		explicit CLuaStateManager()
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
		~CLuaStateManager() = default;

		XPLICIT_COPY_DEFAULT(CLuaStateManager);

	public:
		static CLuaStateManager* get_singleton_ptr() noexcept
		{
			static CLuaStateManager* state = nullptr;

			if (state == nullptr)
				state = new CLuaStateManager();

			return state;
		}

		std::int32_t run(const char* file) noexcept
		{
			if (file)
			{
				const auto ret = luaL_dofile(mL, file);

#ifdef XPLICIT_DEBUG

				if (ret > 0)
					std::cout << lua_tostring(CLuaStateManager::get_singleton_ptr()->state(), -1) << "\n";

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
					std::cout << lua_tostring(CLuaStateManager::get_singleton_ptr()->state(), -1) << "\n";

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

	class XPLICIT_API CLuaClassWizard final
	{
	public:
		explicit CLuaClassWizard(const char* klass) noexcept
			: mClass(klass)
		{
			CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}", mClass).c_str());
		}

		~CLuaClassWizard() = default;

	public:
		XPLICIT_COPY_DEFAULT(CLuaClassWizard);

	public:
		bool insert(const char* symbol, const char* reference)
		{
			if (symbol && reference)
				return CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}.{} = {}", mClass, symbol, reference).c_str());
		
			return false;
		}

	private:
		String mClass;

	};


}
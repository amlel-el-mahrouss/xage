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

#include <vector>

#define XPLICIT_LUA_NAME "CLua"
#define XPLICIT_LUA_DESCRIPTION "Lua for Xplicit"

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

	class XPLICIT_API CLuaClass final
	{
	public:
		explicit CLuaClass(const char* klass) noexcept
			: mClass(klass)
		{
			String fmt = std::format("_G.{}", mClass);
			fmt += " = {}";

			CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		~CLuaClass() noexcept
		{
			CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{} = nil", mClass).c_str());
		}

	public:
		XPLICIT_COPY_DEFAULT(CLuaClass);

	public:
		bool insert(const char* symbol, const char* reference)
		{
			if (symbol && reference)
				return CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}.{} = {}", mClass, symbol, reference).c_str());
		
			return false;
		}

		bool assign(const char* lhs, const char* rhs) { return operator()(lhs, rhs); }
		bool call(const char* lhs) { return operator()(lhs); }
		const char* index(const char* lhs) { return operator[](lhs); }

	public:
		const char* operator[](const char* lhs) noexcept
		{
			lua_pushstring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
				std::format("_G.{}.{};", mClass, lhs).c_str());

			return lua_tostring(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);
		}

		bool operator()(const char* lhs) noexcept
		{
			return Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}.{};", mClass, lhs).c_str());
		}

		bool operator()(const char* lhs, const char* rhs) noexcept
		{
			return Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.{}.{} = {};", mClass, lhs, rhs).c_str());
		}

	private:
		String mClass;

	};

	constexpr const char* XPLICIT_ROOT_CLASS = "_G.Game";

	class CLuaListener final
	{
	public:
		explicit CLuaListener() = default;
		~CLuaListener() = default;

	public:
		XPLICIT_COPY_DEFAULT(CLuaListener);

	public:
		//!
		//! @brief Listens to a specific function.
		//! 0param
		//!		name the string to listen to.
		//! @return if the evaluation was succesful, true.
		
		bool operator()(const String name = ":Idle")
		{
			String fmt = XPLICIT_ROOT_CLASS;
			fmt += name;
			fmt += "()";
		
			return CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

	};

	using ListenerList = std::vector<CLuaListener>;
}
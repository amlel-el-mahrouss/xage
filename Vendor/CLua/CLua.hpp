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

			luaL_openlibs(mL);
		}

	private:
		CLuaStatePtr mL;

	public:
		~CLuaStateManager() = default;

	public:
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
			return (luaL_dofile(mL, file)) < 0;
		}

		std::int32_t run_string(const char* str) noexcept
		{
			return (luaL_dostring(mL, str)) < 0;
		}

		CLuaStatePtr state() noexcept
		{
			return mL;
		}

	};

	// WiP

	class XPLICIT_API CLuaClass
	{
	public:
		explicit CLuaClass(const String& klass) noexcept
			: mClass(klass), mL(CLuaStateManager::get_singleton_ptr()->state()), mCnt(0)
		{
			String fmt = mClass;
			fmt += " = { CLua = true, }";

			luaL_dostring(mL, fmt.c_str());
		}

		virtual ~CLuaClass() noexcept
		{
			CLuaStateManager::get_singleton_ptr()->run_string(std::format("{} = nil", mClass).c_str());
		}

	public:
		XPLICIT_COPY_DEFAULT(CLuaClass);

	public:
		bool insert(const char* symbol, const char* reference)
		{
			if (symbol && reference)
			{
				++mCnt;
				return CLuaStateManager::get_singleton_ptr()->run_string(std::format("{}.{} = {}", mClass, symbol, reference).c_str());
			}

			return false;
		}

		bool assign(const char* lhs, const char* rhs) { return this->insert(lhs, rhs); }

		bool call(const char* lhs) { return operator()(lhs); }

	private:
		//! fast way of fetching a value
		//! execute instruction
		//! get it after that

		bool i_index_field(const char* lhs) noexcept
		{
			std::vector<String> vec;
			String fmt;

			for (size_t i = 0UL; i < (mClass.size() + 1); ++i)
			{
				if (mClass[i] == '.' &&
					!fmt.empty())
				{
					vec.push_back(fmt);
					fmt.clear();

					continue;
				}

				fmt += mClass[i];
			}

			lua_getglobal(mL, vec[0].c_str());

			if (!lua_istable(mL, -1))
				return false;

			for (size_t i = 1UL; i < vec.size(); ++i)
			{
				lua_pushstring(mL, vec[i].c_str());
				lua_gettable(mL, -1);

				if (lua_istable(mL, -1))
				{
					lua_getfield(mL, -1, lhs);
					return true;
				}

				lua_pop(mL, 1);
			}
		}

		void i_clean(const std::size_t& cnt) noexcept
		{
			lua_pop(mL, cnt);
		}

	public:
		template <typename T = double>
		const T index_as_number(const char* lhs) noexcept
		{
			T ret = 0;

			if (this->i_index_field(lhs))
			{
				ret = lua_tonumber(mL, -1);
			}

			this->i_clean(1);
			return ret;
		}

		const bool index_as_bool(const char* lhs)
		{
			bool ret = false;

			if (this->i_index_field(lhs))
			{
				ret = lua_toboolean(mL, -1);
			}

			this->i_clean(1);
			return ret;
		}

		const String index_as_string(const char* lhs)
		{
			String ret = "";

			if (this->i_index_field(lhs))
			{
				ret = lua_tostring(mL, -1);
			}

			this->i_clean(1);
			return ret;
		}

		bool operator()(const char* lhs) noexcept
		{
			if (!lhs)
				return false;

			return (luaL_dostring(mL, lhs)) < 0;
		}

		bool operator()(const char* lhs, const char* rhs) noexcept
		{
			if (!lhs ||
				!rhs)
				return false;

			return (luaL_dostring(mL, lhs)) < 0;
		}

		std::int64_t count() { return mCnt; }

	private:
		std::int64_t mCnt;
		CLuaStatePtr mL;
		String mClass;

	};

	constexpr const char* XPLICIT_ROOT_CLASS = "_G.World";

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
		//! 		
		
		bool operator()(const String name = ":Tick")
		{
			String fmt = XPLICIT_ROOT_CLASS;
			fmt += name;
			fmt += "()";
		
			return CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

	};

	using ListenerList = std::vector<CLuaListener>;
}
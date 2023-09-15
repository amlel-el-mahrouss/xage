/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

 /* Author: Amlal El Mahrouss */
 /* C++Lua for the XplicitNgine. */

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

#ifndef __XPLICIT_H__
#	include <NginCore.h>
#endif // ifndef __XPLICIT_H__

#include <vector>

#define CLUA_USER_DATA_SYMBOL "UserData"

#define XPLICIT_LUA_NAME "C++Lua"
#define XPLICIT_LUA_DESCRIPTION "C++ Lua extension for the XplicitNgine."
#define XPLICIT_LUA_AUTHOR "Amlal El Mahrouss"

namespace XPX::Lua
{
	enum
	{
		ReadOnly
	};

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

	public:
		void global_set(lua_CFunction func, const String name) noexcept
		{
			lua_pushcfunction(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), func);
			lua_setglobal(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), name.c_str());
		}

		std::int32_t run(const String file) noexcept
		{
			if (file.empty())
				return -1;

			if (auto err = (luaL_dofile(mL, file.c_str())) > 0)
			{
				String _err = lua_tostring(mL, -1);

				return err;
			}

			return 0;
		}

		std::int32_t run_string(const String str) noexcept
		{
			if (str.empty())
				return -1;

			if (auto err = (luaL_dostring(mL, str.c_str())); err)
			{
				String _err = lua_tostring(mL, -1);
				XPLICIT_ERROR(_err);

				return err;
			}

			return 0;
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
			: mClass(klass), mL(CLuaStateManager::get_singleton_ptr()->state()), mSymbolCnt(0)
		{
			luaL_dostring(mL, fmt::format("{} = {}", klass, "{}").c_str());
		}

		virtual ~CLuaClass() noexcept = default;

	public:
		XPLICIT_COPY_DEFAULT(CLuaClass);

	public:
		bool insert_userdata(const String symbol,
			void* value)
		{
			if (!symbol.empty() &&
				value)
			{
				lua_pushstring(this->state(), mClass.c_str());

				if (lua_gettable(mL, -1) == LUA_OK)
				{
					mSymbols.push_back(std::make_pair(mSymbolCnt, CLUA_USER_DATA_SYMBOL));
					++mSymbolCnt;

					lua_newtable(mL);

					lua_pushcfunction(mL, [](lua_State* L) -> int { lua_error(L); return 0; });
					lua_setfield(mL, -2, "__newindex");

					lua_pushlightuserdata(mL, value);
					lua_setfield(mL, -2, "__CxxData");
					lua_setfield(mL, -1, symbol.c_str());

					return true;
				}
			}

			return false;
		}

		bool insert(const String symbol, const String value)
		{
			if (!symbol.empty() &&
				!value.empty())
			{
				if (this->run_string(fmt::format("{}.{} = {}", mClass, symbol, value)).empty())
					return true;
			}

			return false;
		}

		bool assign(const String lhs, const String rhs) 
		{ 
			auto fmt = fmt::format("{}.{} = {}", mClass, lhs, rhs);
			return luaL_dostring(mL, fmt.c_str()) == 0;
		}

	private:
		//! @brief Index field of an array.
		bool i_index_field(const String lhs) noexcept
		{
			if (lhs.empty())
				return false;

			String fmt = "return ";

			fmt += mClass;
			fmt += ".";
			fmt += lhs;

			auto ret = luaL_dostring(mL, fmt.c_str()) == 0;

			if (!ret)
			{
				XPLICIT_CRITICAL(lua_tostring(mL, -1));
				return false;
			}

			return true;
		}

		void i_clean(const std::int64_t& cnt) noexcept
		{
			lua_pop(mL, cnt);
		}

	public:
		template <typename T = double>
		const T index_as_number(const String lhs) noexcept
		{
			T ret = -1;

			if (this->i_index_field(lhs))
			{
				if (lua_isnumber(mL, -1))
				{
					ret = lua_tonumber(mL, -1);
				}
			}

			return ret;
		}

		const bool index_as_bool(const String lhs)
		{
			bool ret = false;

			if (this->i_index_field(lhs))
			{
				if (lua_isboolean(mL, -1))
				{
					ret = lua_toboolean(mL, -1);
				}
			}

			return ret;
		}

		lua_State* state() noexcept { return mL; }

		const String index_as_string(const String lhs)
		{
			String ret = "";

			if (this->i_index_field(lhs))
			{
				if (lua_isstring(mL, -1))
				{
					ret = lua_tostring(mL, -1);
				}
			}

			return ret;
		}

		const String run_string(const String lhs) noexcept
		{
			if (lhs.empty())
				return "";

			auto ret = (luaL_dostring(mL, lhs.c_str())) == 0;

			if (!ret)
			{
				String ret_err = lua_tostring(mL, -1);
				XPLICIT_ERROR(ret_err);

				return ret_err;
			}

			return "";
		}

		const String run_path(const String lhs) noexcept
		{
			if (lhs.empty())
				return "";

			auto ret = (luaL_dofile(mL, lhs.c_str())) == 0;
			
			if (!ret)
			{
				String ret_err = lua_tostring(mL, -1);
				XPLICIT_ERROR(ret_err);

				return ret_err;
			}
		
			return "";
		}

		const String call_method(const String method) noexcept
		{
			if (method.empty())
				return "";

			String fmt = mClass;
			fmt += ":";
			fmt += method;

			return this->run_string(fmt.c_str());
		}

		std::int64_t count() { return mSymbolCnt; }
		std::vector<std::pair<std::int64_t, std::string>>& symbols() { return mSymbols; }

	private:
		std::vector<std::pair<std::int64_t, std::string>> mSymbols;
		std::size_t mSymbolCnt;
		CLuaStatePtr mL;
		String mClass;

	};
}

#include <RLua.hpp>


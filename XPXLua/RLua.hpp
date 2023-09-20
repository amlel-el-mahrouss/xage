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
 /* ReflectedLua for the XplicitNgine. */

#define XPLICIT_RLUA_NAME "RLua"
#define XPLICIT_RLUA_DESCRIPTION "RLua extension for the XPXNgine."
#define XPLICIT_RLUA_AUTHOR "Amlal El Mahrouss"

#define RLUA_ASSERT(X) XPLICIT_ASSERT(X)

// L = lua_State, N = index, X = name.
#define RLUA_TYPE_CHECK(L, N, X) XPX::String index = luaL_checkstring(L, N); luaL_argcheck(L, index == X, 2, "RLua: Index out of range")

namespace XPX::RLua
{
	typedef int (*RLuaProc)(lua_State* L);

	template <typename Class>
	class RuntimeClass final : public Class
	{
	public:
		RuntimeClass() = default;
		~RuntimeClass() = default;

	private:
		static int on_new(lua_State* L)
		{
			*reinterpret_cast<Class**>(lua_newuserdata(L, sizeof(Class*))) = new Class();

			return 1;
		}

		// Free RuntimeClass instance by Lua garbage collection
		static int on_delete(lua_State* L)
		{
			XPLICIT_INFO("Class::on_delete");
			delete_internal(lua_touserdata(L, 1));
			
			return 0;
		}

		static void delete_internal(void* block)
		{
#ifdef XPLICIT_WINDOWS
#ifdef _DEBUG
			_free_dbg(block, _UNKNOWN_BLOCK);
#else
			free(block);
#endif
#else
			free(block);
#endif
		}

	public:
		RuntimeClass& begin_class() noexcept
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			luaL_newmetatable(L, Class::name());

			lua_pushcfunction(L, &RuntimeClass<Class>::on_delete);
			lua_setfield(L, -2, "__gc");

			lua_pushcfunction(L, &RuntimeClass<Class>::on_delete);
			lua_setfield(L, -2, "release");

			lua_pushcfunction(L, &RuntimeClass<Class>::on_new);
			lua_setfield(L, -2, "borrow");

			mName = Class::name();

			return *this;
		}

		RuntimeClass& append_proc(String fn_name, RLuaProc fn) noexcept
		{
			RLUA_ASSERT(fn && !fn_name.empty());

			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_pushcfunction(L, fn);
			lua_setfield(L, -2, fn_name.c_str());

			return *this;
		}

		RuntimeClass& append_prop(String name, RLuaProc getter, RLuaProc setter) noexcept
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			luaL_newmetatable(L, name.c_str());

			lua_pushcfunction(L, getter);
			lua_setfield(L, -3, "__index");

			lua_pushcfunction(L, setter);
			lua_setfield(L, -3, "__newindex");

			lua_setfield(L, -2, name.c_str());

			return *this;
		}

		RuntimeClass& end_class() noexcept
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");

			lua_setmetatable(L, -2);

			return *this;
		}

	private:
		std::vector<std::tuple<String, lua_CFunction>> mFuncs;

	private:
		String mName;

	};
}
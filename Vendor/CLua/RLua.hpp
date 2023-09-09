/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

 /* Author: Amlal El Mahrouss */
 /* ReflectedLua for the XplicitNgine. */

#define XPLICIT_RLUA_NAME "ReflectedLua"
#define XPLICIT_RLUA_DESCRIPTION "ReflectedLua extension for the XplicitNgine."
#define XPLICIT_RLUA_AUTHOR "Amlal El Mahrouss"

namespace Xplicit::RLua
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
			*reinterpret_cast<RuntimeClass**>(lua_newuserdata(L, sizeof(RuntimeClass*))) = new RuntimeClass();
			
			return 1;
		}

		// Free RuntimeClass instance by Lua garbage collection
		static int on_delete(lua_State* L)
		{
			delete reinterpret_cast<RuntimeClass**>(lua_touserdata(L, 1));

			return 0;
		}

	public:
		RuntimeClass& begin_class(const char* name)
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_newtable(L);
			lua_pushcfunction(L, RuntimeClass<Class>::on_new);

			lua_setfield(L, -2, "New");
			lua_setglobal(L, name);

			luaL_newmetatable(L, name);

			lua_pushcfunction(L, RuntimeClass<Class>::on_delete);

			lua_setfield(L, -2, "__gc");

			lua_pushvalue(L, -1); 
			lua_setfield(L, -2, "__index");

			return *this;
		}

		RuntimeClass& append_proc(const char* fn_name, RLuaProc fn)
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_pushcfunction(L, fn);
			lua_setfield(L, -2, fn_name);

			return *this;
		}

		RuntimeClass& end_class()
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_pop(L, 1);

			return *this;
		}

	};
}
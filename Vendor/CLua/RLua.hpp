/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

 /* Author: Amlal El Mahrouss */
 /* ReflectedLua for the XplicitNgine. */

#define XPLICIT_RLUA_NAME "ReflectedLua"
#define XPLICIT_RLUA_DESCRIPTION "ReflectedLua extension for the XplicitNgine."
#define XPLICIT_RLUA_AUTHOR "Amlal El Mahrouss"

// L = lua_State, N = index, X = name.
#define RLUA_TYPE_CHECK(L, N, X) XPX::String index = luaL_checkstring(L, N);luaL_argcheck(L, index == X, 2, "RLua: Index out of range");

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
			*reinterpret_cast<RuntimeClass**>(lua_newuserdata(L, sizeof(RuntimeClass*))) = new RuntimeClass();
			
			return 1;
		}

		// Free RuntimeClass instance by Lua garbage collection
		static int on_delete(lua_State* L)
		{
			delete reinterpret_cast<RuntimeClass**>(lua_touserdata(L, 1));

			return 0;
		}

		static int delete_internal(void* block)
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
		RuntimeClass& begin_class(const char* name)
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_register(L, name, RuntimeClass<Class>::on_new);

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

		RuntimeClass& append_prop(const char* prop_name, RLuaProc getter, RLuaProc setter)
		{
			auto L = Lua::CLuaStateManager::get_singleton_ptr()->state();

			lua_newtable(L);

			*this = this->append_proc("__index", getter);
			*this = this->append_proc("__newindex", setter);

			lua_setfield(L, -1, prop_name);

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
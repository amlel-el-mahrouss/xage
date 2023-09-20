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

			luaL_setmetatable(L, Class::name());

			XPLICIT_INFO("Class::on_new");
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
			mL = Lua::CLuaStateManager::get_singleton_ptr()->state();

			luaL_newmetatable(mL, Class::name());

			lua_pushvalue(mL, -1);
			lua_setfield(mL, -2, "__index");

			lua_pushstring(mL, Class::name());
			lua_setfield(mL, -2, "__tostring");

			lua_pushvalue(mL, -1);
			lua_setfield(mL, -2, "__metatable");

			*this = this->append_proc("__gc", &RuntimeClass<Class>::on_delete);
			*this = this->append_proc("borrow", &RuntimeClass<Class>::on_new);
			*this = this->append_proc("release", &RuntimeClass<Class>::on_delete);

			mName = Class::name();

			return *this;
		}

		RuntimeClass& append_proc(String fn_name, RLuaProc fn) noexcept
		{
			lua_pushcfunction(mL, fn);
			lua_setfield(mL, -2, fn_name.c_str());

			return *this;
		}

		RuntimeClass& end_class() noexcept
		{
			lua_setmetatable(mL, -2);

			return *this;
		}

	private:
		std::vector<std::tuple<String, lua_CFunction>> mFuncs;

	private:
		lua_State* mL;
		String mName;

	};
}
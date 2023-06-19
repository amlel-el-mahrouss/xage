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
/* XLua for C++ classes. */

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
}

namespace Xplicit::Lua
{
	template <typename T>
	class ILuaClass final : public T
	{
	public:
		ILuaClass() = default;

	public:
		ILuaClass& operator=(const ILuaClass&) = default;
		ILuaClass(const ILuaClass&) = default;

	public:
		template <typename StateManager, typename MethodTranslater>
		void register_class(MethodTranslater& methods, const std::string& name) noexcept
		{
			luaL_newmetatable(StateManager::get_singleton_ptr(), name.c_str());
			lua_pushstring(StateManager::get_singleton_ptr(), "_index");

			methods<StateManager, T>(this);

			lua_setglobal(StateManager::get_singleton_ptr()->state(), name.c_str());
		}

	};
}
/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "InstanceComponent.h"
#include "HelperMacros.h"
#include "Xplicit.h"

#include <Nplicit.h>

#include <lua/lua.hpp>


static int lua_InstanceNew(lua_State* L)
{
	Xplicit::String name = lua_tostring(L, 1);
	Xplicit::String parent = lua_tostring(L, 2);
	Xplicit::String script = lua_tostring(L, 3);
	bool rw = lua_toboolean(L, 4);

	if (name.empty() ||
		parent.empty())
		return 0;

	auto instance = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::InstanceComponent>(Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Color<float>(0, 0, 0),
		script.c_str(),
		parent.c_str(),
		name.c_str(),
		rw);

	return 0;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.CoreAPI = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game.Players = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_InstanceNew);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.CoreAPI.New");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
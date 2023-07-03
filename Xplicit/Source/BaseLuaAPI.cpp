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

static int lua_New(lua_State* L)
{
	Xplicit::String name = lua_tostring(L, 1);
	Xplicit::String parent = lua_tostring(L, 2);
	Xplicit::String script = lua_tostring(L, 3);
	bool rw = lua_toboolean(L, 4);

	if (name.empty() ||
		parent.empty())
		return 0;

	auto instance = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::InstanceComponent>(
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Color<float>(0, 0, 0),
		script.c_str(),
		parent.c_str(),
		name.c_str(),
		rw
	);

	XPLICIT_ASSERT(instance);

	return 0;
}

static int lua_Destroy(lua_State* L)
{
	Xplicit::String name = lua_tostring(L, 1);
	Xplicit::String parent = lua_tostring(L, 2);

	if (name.empty())
		return 0;

	if (parent.empty())
		return 0;

	if (auto instance = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::InstanceComponent>(name.c_str()))
	{
		if (instance->parent() == parent)
			Xplicit::ComponentManager::get_singleton_ptr()->remove<Xplicit::InstanceComponent>(instance);
	}

	return 0;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.CoreAPI = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.CoreAPI.SceneService = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game.Players = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_New);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "New_CoreAPI");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.CoreAPI.SceneService.New = New_CoreAPI");
	
	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_Destroy);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Destroy_CoreAPI");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.CoreAPI.SceneService.Destroy = Destroy_CoreAPI");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
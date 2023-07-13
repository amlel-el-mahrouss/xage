/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ClassComponent.h"
#include "HelperMacros.h"
#include "Xplicit.h"

#include <Nplicit.h>
#include <CLua/CLua.hpp>

static int lua_New(lua_State* L)
{
	Xplicit::String name;
	Xplicit::String parent;
	Xplicit::String script;

	name += lua_tostring(L, 1);
	parent += lua_tostring(L, 2);

	script += lua_tostring(L, 3);
	
	if (name.empty() ||
		script.empty())
	{
		lua_pushboolean(L, false);
		return 1;
	}

	auto instance = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::ClassComponent>(
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Color<float>(0, 0, 0),
		script.c_str(),
		parent.c_str(),
		name.c_str()
	);

	XPLICIT_ASSERT(instance);

	
	lua_pushboolean(L, true);
	return 1;
}

static int lua_Destroy(lua_State* L)
{
	Xplicit::String name;
	Xplicit::String parent;

	name += lua_tostring(L, 1);
	parent += lua_tostring(L, 2);

	if (name.empty())
	{
		lua_pushboolean(L, false);
		return 1;
	}

	if (auto instance = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::ClassComponent>(name.c_str()))
	{
		if (instance->parent() == parent)
		{
			if (instance->get_attribute().script())
				Xplicit::ComponentSystem::get_singleton_ptr()->remove(instance->get_attribute().script());

			Xplicit::ComponentSystem::get_singleton_ptr()->remove<Xplicit::ClassComponent>(instance);
		}
	}

	lua_pushboolean(L, true);
	return 1;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Script = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World = {}");
	
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.Settings = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ClassService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.Players = {}");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_New);
	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "Engine_Create");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ClassService.Create = Engine_Create");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_Destroy);
	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "Engine_Destroy");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ClassService.Destroy = Engine_Destroy");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "XplicitPreload.lua";

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
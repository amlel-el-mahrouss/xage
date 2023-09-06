/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"
#include "ClassComponent.h"

#include "HelperMacros.h"
#include "Xplicit.h"

#include <Nplicit.h>
#include <CLua/CLua.hpp>

static int lua_New(lua_State* L)
{
	if (!lua_tostring(L, 1) ||
		!lua_tostring(L, 2))
	{
		lua_pushboolean(L, false);
		return 1;
	}

	Xplicit::String name;
	Xplicit::String parent;
	Xplicit::String script;

	name += lua_tostring(L, 1);
	parent += lua_tostring(L, 2);
	script += lua_tostring(L, 3) ? lua_tostring(L, 3) : "";
	
	if (name.empty() ||
		parent.empty())
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
		if (instance->parent() == parent ||
			instance->name() == name)
		{
			if (instance->script())
				Xplicit::ComponentSystem::get_singleton_ptr()->remove(instance->script());

			Xplicit::ComponentSystem::get_singleton_ptr()->remove<Xplicit::ClassComponent>(instance);
		}
	}

	lua_pushboolean(L, true);
	return 1;
}

static int lua_Wait(lua_State* L)
{
	int seconds = lua_tonumber(L, 1);

	std::this_thread::sleep_for(std::chrono::seconds(seconds));

	return 0;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Script = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Color3 = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Vector3 = {}");

	// have a look at GameVar if it ever crashes.
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.Settings = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.Players = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService = {}");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_Wait);
	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "Wait");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ClassService = {}");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_New);
	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineCreateClass");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ClassService.New = EngineCreateClass");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_Destroy);
	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineDestroy");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ClassService.Destroy = EngineDestroyClass");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "XplicitPreload.lua";

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
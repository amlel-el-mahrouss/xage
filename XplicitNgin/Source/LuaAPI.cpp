/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"
#include "PartComponent.h"

#include "HelperMacros.h"
#include "Xplicit.h"

#include <Nplicit.h>
#include <CLua/CLua.hpp>

#include "LuaAPI.h"

static int lua_CreateClass(lua_State* L)
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

	auto klass = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::ClassComponent>(
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Vector<float>(0, 0, 0),
		Xplicit::Color<float>(0, 0, 0),
		script.c_str(),
		parent.c_str(),
		name.c_str()
	);

	XPLICIT_ASSERT(klass);

	lua_pushboolean(L, true);
	return 1;
}

static int lua_DestroyClass(lua_State* L)
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

	if (auto klass = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::ClassComponent>(name.c_str()))
	{
		if (klass->parent() == parent ||
			klass->name() == name)
		{
			if (klass->script())
				Xplicit::ComponentSystem::get_singleton_ptr()->remove(klass->script());

			Xplicit::ComponentSystem::get_singleton_ptr()->remove<Xplicit::ClassComponent>(klass);
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

static int lua_CreatePart(lua_State* L)
{
	const char* name = lua_tostring(L, 1);
	const char* parent = lua_tostring(L, 2);

	Xplicit::PartComponent* part = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::PartComponent>(name, parent);

	if (part)
	{
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushboolean(L, false);
	return 0;
}

static int lua_DestroyPart(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	Xplicit::PartComponent* part = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::PartComponent>(name);

	if (part)
		Xplicit::ComponentSystem::get_singleton_ptr()->remove(part);

	return 0;
}

static int lua_DestroyScript(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	Xplicit::LuaScriptComponent* script = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::LuaScriptComponent>(name);

	if (script)
		Xplicit::ComponentSystem::get_singleton_ptr()->remove(script);

	return 0;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("Script = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World = {}");

	// have a look at GameVar if it ever crashes.
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World.Settings = {}");
	
	// players table.
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World.Players = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Wait, "Wait");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_CreateClass, "XPXCreateClass");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyClass, "XPXDestroyClass");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_CreatePart, "XPXCreatePart");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyPart, "XPXDestroyPart");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyScript, "XPXDestroyScript");

	Xplicit::RLua::RuntimeClass<Vector3> vector;

	vector.begin_class("Vector3").
		append_proc("__add", &Vector3::add)
		.append_proc("__mul", &Vector3::mul)
		.append_proc("__sub", &Vector3::sub)
		.append_prop("X", &Vector3::get_x, &Vector3::set_x)
		.append_prop("Y", &Vector3::get_y, &Vector3::set_y)
		.append_prop("Z", &Vector3::get_z, &Vector3::set_z)
		.end_class();

	XPLICIT_GET_DATA_DIR(full_path);
	
	Xplicit::String xpx_shared_base = full_path;

	xpx_shared_base += "Contents/";
	xpx_shared_base += "XPXSharedBase.lua";

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run(xpx_shared_base.c_str());
}
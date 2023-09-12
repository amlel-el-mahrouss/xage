/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"
#include "PartComponent.h"

#include "HelperMacros.h"
#include "NginCore.h"

#include <Nplicit.h>
#include <CLua.hpp>

#include "LuaAPI.h"

static int lua_CreateClass(lua_State* L)
{
	if (!lua_tostring(L, 1) ||
		!lua_tostring(L, 2))
	{
		lua_pushboolean(L, false);
		return 1;
	}

	XPX::String name;
	XPX::String parent;
	XPX::String script;

	name += lua_tostring(L, 1);
	parent += lua_tostring(L, 2);
	script += lua_tostring(L, 3) ? lua_tostring(L, 3) : "";
	
	if (name.empty() ||
		parent.empty())
	{
		lua_pushboolean(L, false);
		return 1;
	}

	auto klass = XPX::ComponentSystem::get_singleton_ptr()->add<XPX::ClassComponent>(
		XPX::Vector<float>(0, 0, 0),
		XPX::Vector<float>(0, 0, 0),
		XPX::Color<float>(0, 0, 0),
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
	XPX::String name;
	XPX::String parent;

	name += lua_tostring(L, 1);
	parent += lua_tostring(L, 2);

	if (name.empty())
	{
		lua_pushboolean(L, false);
		return 1;
	}

	if (auto klass = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::ClassComponent>(name.c_str()))
	{
		if (klass->parent() == parent ||
			klass->name() == name)
		{
			if (klass->script())
				XPX::ComponentSystem::get_singleton_ptr()->remove(klass->script());

			XPX::ComponentSystem::get_singleton_ptr()->remove<XPX::ClassComponent>(klass);
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

	XPX::PartComponent* part = XPX::ComponentSystem::get_singleton_ptr()->add<XPX::PartComponent>(name, parent);

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

	XPX::PartComponent* part = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::PartComponent>(name);

	if (part)
		XPX::ComponentSystem::get_singleton_ptr()->remove(part);

	return 0;
}

static int lua_DestroyScript(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	XPX::LuaScriptComponent* script = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::LuaScriptComponent>(name);

	if (script)
		XPX::ComponentSystem::get_singleton_ptr()->remove(script);

	return 0;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("Script = {}");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World = {}");

	// have a look at GameVar if it ever crashes.
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World.Settings = {}");
	
	// players table.
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World.Players = {}");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Wait, "Wait");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_CreateClass, "XPXCreateClass");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyClass, "XPXDestroyClass");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_CreatePart, "XPXCreatePart");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyPart, "XPXDestroyPart");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyScript, "XPXDestroyScript");

	XPLICIT_GET_DATA_DIR(full_path);
	
	XPX::String xpx_shared_base = full_path;

	xpx_shared_base += "Contents/";
	xpx_shared_base += "XPXSharedBase.lua";

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run(xpx_shared_base.c_str());
}
/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"

#include "HelperMacros.h"
#include "NginCore.h"

#include <Nplicit.h>
#include <CLua.hpp>

#include "LuaAPI.h"

static int lua_DestroyScript(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	XPX::LuaScriptComponent* script = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::LuaScriptComponent>(name);

	if (script)
	{
		XPLICIT_INFO("Removing LuaScriptComponent...");
		XPX::ComponentSystem::get_singleton_ptr()->remove(script);
	}

	return 0;
}

static int lua_Info(lua_State* L)
{
	if (lua_isstring(L, 1))
	{
		XPLICIT_INFO(lua_tostring(L, 1));
	}

	return 0;
}

XPLICIT_API void XplicitLoadBaseLua()
{
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("Script = {}");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("world = {}");
	
	// have a look at GameVar if it ever crashes.
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("world.Settings = {}");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("world.Players = {}");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyScript, "destroyScript");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Info, "info");

	XPLICIT_GET_DATA_DIR(full_path);

	XPX::String xpx_shared_base = full_path;

	xpx_shared_base += "Contents/";
	xpx_shared_base += "XPXSharedBase.lua";

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run(xpx_shared_base.c_str());
}
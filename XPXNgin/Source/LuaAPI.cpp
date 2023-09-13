/*
 * =====================================================================
 *
 *			XPXServer
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

static int lua_Wait(lua_State* L)
{
	int seconds = lua_tonumber(L, 1);

	std::this_thread::sleep_for(std::chrono::seconds(seconds));

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

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World.Settings = {}");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World.Players = {}");

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Wait, "Wait");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyScript, "XPXDestroyScript");


}
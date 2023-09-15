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
		XPX::ComponentSystem::get_singleton_ptr()->remove(script);

	return 0;
}

static int lua_Wait(lua_State* L)
{
	int wait_for = lua_tointeger(L, 1);
	const char* script_id = lua_tostring(L, 2);
	
	if (script_id)
	{
		auto scripts = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::LuaScriptComponent>("LuaScriptComponent");

		for (auto& script : scripts)
		{
			if (strncmp(script->path(), script_id, strlen(script->path()) == 0))
			{
				SuspendThread((HANDLE)script->leak_thread().native_handle());
				
				XPX::Thread sleep_job([&]() {
					HANDLE winthread = (HANDLE)script->leak_thread().native_handle();
					std::this_thread::sleep_for(std::chrono::seconds(wait_for));
					ResumeThread(winthread);
				});

				sleep_job.detach();

				return 0;
			}
		}

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

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Wait, "wait");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyScript, "destroyScript");

	XPLICIT_GET_DATA_DIR(full_path);

	XPX::String xpx_shared_base = full_path;

	xpx_shared_base += "Contents/";
	xpx_shared_base += "XPXSharedBase.lua";

	XPX::Lua::CLuaStateManager::get_singleton_ptr()->run(xpx_shared_base.c_str());
}
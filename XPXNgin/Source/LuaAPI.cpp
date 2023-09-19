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

class XPXUri
{
public:
	XPX::Utils::UriParser Uri = { XPLICIT_XASSET_PROTOCOL };

	XPXUri() {}
	~XPXUri() = default;

public:
	XPLICIT_COPY_DEFAULT(XPXUri);

public:
	static int parse_url(lua_State* L)
	{
		XPXUri* uri = (XPXUri*)lua_touserdata(L, 1);

		XPX::String uri_str = lua_tostring(L, 2);
		uri_str = uri_str.erase(uri_str.find(XPLICIT_XASSET_PROTOCOL), strlen(XPLICIT_XASSET_PROTOCOL));

		if (!uri_str.empty())
		{
			uri->Uri /= uri_str;

			lua_pushstring(L, uri->Uri.get().c_str());
			return 1;
		}

		lua_pushstring(L, "invalid-asset");
		return 1;
	}

};

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
	XPX::RLua::RuntimeClass<XPXUri> uri;
	uri.begin_class("Uri").append_proc("Parse", &XPXUri::parse_url).end_class();

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
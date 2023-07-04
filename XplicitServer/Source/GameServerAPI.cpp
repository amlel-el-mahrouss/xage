/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationManager.h"
#include "HumanoidComponent.h"

#include <lua/lua.hpp>
#include <RoXML.h>

 // RoXML parser
Xplicit::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

static int lua_SetHumanoidHealth(lua_State* L)
{
	const auto humanoid = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (humanoid.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);
	const int64_t dmg = lua_tointeger(L, 2);

	if (id == -1)
		return 0;

	for (std::size_t index = 0; index < humanoid.size(); index++)
	{
		if (humanoid[index]->id() == id)
		{
			humanoid[index]->set_health(dmg);
			break;
		}
	}

	return 0;
}

static int lua_LoadScript(lua_State* L)
{
	const char* path = lua_tostring(L, 1);

	// player-id because you send this to players.
	const int64_t hash = lua_tointeger(L, 1);

	if (path == nullptr)
		return 1;

	if (hash == XPLICIT_INVALID_HASH)
		return 1;

	// must be an xasset://
	Xplicit::ServerReplicationManager::get_singleton_ptr()->create(Xplicit::COMPONENT_ID_SCRIPT, path, hash);

	return 0;
}

static int lua_LoadRoXML(lua_State* L)
{
	auto _path = lua_tostring(L, 1);

	Xplicit::RoXML::RoXMLDocumentParameters params;

	params.Has3D = false;
	params.LuaOnly = false;
	params.NoLua = false;

	params.Path = _path;

	if (params.Path.empty())
	{
		lua_pushnil(L);
		return 1;
	}

	XPLICIT_PARSER.load(params);

	return 0;
}

static int lua_Kick(lua_State* L)
{
	const auto humanoid = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (humanoid.empty())
		return 0;

	auto id = lua_tostring(L, 1);

	for (std::size_t index = 0; index < humanoid.size(); index++)
	{
		if (humanoid[index]->get_peer() &&
			humanoid[index]->get_peer()->xplicit_id.as_string() == id)
		{
			humanoid[index]->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = Xplicit::NETWORK_CMD_KICK;
			return 0;
		}
	}

	return 0;
}

void XplicitLoadServerLua() noexcept
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.HumanoidService = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.ScriptService = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.NetworkService = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.RoXMLService = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetHumanoidHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "CoreAPI_HumanoidHealth");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.HumanoidService.Health = CoreAPI_HumanoidHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_LoadScript);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "CoreAPI_LoadScript");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.ScriptService.Load = CoreAPI_LoadScript");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "CoreAPI_LoadRoXML");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.RoXMLService.Load = CoreAPI_LoadRoXML");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_Kick);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "CoreAPI_Kick");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.NetworkService.Kick = CoreAPI_Kick");

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game.Players = {}");
}
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

#include <lua/CLua.hpp>
#include <RoXML.h>

 // RoXML parser
Xplicit::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

static int lua_LoadScript(lua_State* L)
{
	const char* path = lua_tostring(L, 1);

	// player-id because you send this to players.
	const auto id = lua_tostring(L, 1);

	if (path == nullptr ||
		id == nullptr)
	{
		lua_pushnil(L);
		return 1;
	}

	const auto humanoids = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	for (size_t i = 0; i < humanoids.size(); ++i)
	{
		if (humanoids[i]->get_peer() &&
			humanoids[i]->get_peer()->xplicit_id.as_string() == id)
		{
			// must be an xasset:// !
			Xplicit::ServerReplicationManager::get_singleton_ptr()->create(Xplicit::COMPONENT_ID_SCRIPT, path, humanoids[i]->get_peer()->hash);

			break;
		}
	}

	
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

static int lua_NetworkService_Kick(lua_State* L)
{
	const auto humanoid = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (humanoid.empty())
		return 0;

	auto id = lua_tostring(L, 1);

	for (std::size_t index = 0; index < humanoid.size(); index++)
	{
		if (humanoid[index]->get_peer() &&
			humanoid[index]->get_peer()->xplicit_id.as_string() == id)
		{
			humanoid[index]->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = Xplicit::NETWORK_CMD_KICK;
			break;
		}
	}

	
	return 0;
}

static int lua_NetworkService_Create(lua_State* L)
{
	auto string = lua_tostring(L, 1);
	int32_t repl_id = lua_tointeger(L, 2);
	Xplicit::String id = lua_tostring(L, 3);

	if (string == nullptr ||
		*string == 0 ||
		repl_id > Xplicit::COMPONENT_ID_COUNT)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	const auto humanoids = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	for (size_t i = 0; i < humanoids.size(); ++i)
	{
		if (humanoids[i]->get_peer() &&
			humanoids[i]->get_peer()->xplicit_id.as_string() == id)
		{
			Xplicit::ServerReplicationManager::get_singleton_ptr()->create(repl_id, string, humanoids[i]->get_peer()->hash);

			break;
		}
	}

	
	lua_pushboolean(L, true);

	return 1;
}

void XplicitLoadServerLua() noexcept
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.ReplicationService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.ScriptService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.RoXMLService = {}");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadScript);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "GameAPI_LoadScript");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.ScriptService.Load = GameAPI_LoadScript");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "GameAPI_LoadRoXML");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.RoXMLService.Load = GameAPI_LoadRoXML");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_NetworkService_Kick);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "GameAPI_Kick");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.ReplicationService.Kick = GameAPI_Kick");
	
	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_NetworkService_Create);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "Engine_Create");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.Game.ReplicationService.Create = Engine_Create");
}
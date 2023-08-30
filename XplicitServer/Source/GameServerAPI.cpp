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
#include "GearComponent.h"

#include <CLua/CLua.hpp>
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

	XPLICIT_PARSER.parse(params);

	return 0;
}

static int lua_NetworkService_Fire(lua_State* L)
{
	auto string = lua_tostring(L, 1);
	int32_t repl_id = lua_tointeger(L, 2);
	Xplicit::String id = lua_tostring(L, 3);

	//! either create, update, remove
	int64_t create_update_remove = lua_tonumber(L, 4);

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
			switch (create_update_remove)
			{
			case 0:
			{
				Xplicit::ServerReplicationManager::get_singleton_ptr()->create(repl_id, string, humanoids[i]->get_peer()->hash);
				break;
			}
			case 1:
			{
				Xplicit::ServerReplicationManager::get_singleton_ptr()->update(repl_id, string, humanoids[i]->get_peer()->hash);
				break;
			}
			case 2:
			{
				Xplicit::ServerReplicationManager::get_singleton_ptr()->remove(repl_id, string, humanoids[i]->get_peer()->hash);
				break;
			}
			}

			break;
		}
	}

	lua_pushboolean(L, true);
	return 1;
}

static int lua_CreateGear(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	Xplicit::GearComponent* gear = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::GearComponent>(name, "World");

	return 0;
}

void XplicitLoadServerLua() noexcept
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ReplicationService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.InventoryService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ScriptService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService = {}");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_CreateGear);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineCreateGear");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService.Create = EngineCreateGear");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadScript);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineLoadScript");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ScriptService.Load = EngineLoadScript");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineLoadRoXML");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService.Load = EngineLoadRoXML");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_NetworkService_Fire);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineFireServer");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.ReplicationService.Fire = EngineFireServer");
}
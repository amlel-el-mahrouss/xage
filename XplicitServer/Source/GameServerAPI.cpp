/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationFactory.h"
#include "HumanoidComponent.h"
#include "GearComponent.h"

#include <CLua/CLua.hpp>
#include <RoXML.h>

 // RoXML parser
Xplicit::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

static int lua_LoadRoXML(lua_State* L)
{
	auto _path = lua_tostring(L, 1);
	auto _client = lua_toboolean(L, 2);

	if (!_client)
	{
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
	}
	else
	{
		lua_rawgeti(L, 3, 3);

		auto xid = lua_tostring(L, -3);

		Xplicit::ServerReplicationFactory factory;
		std::vector<Xplicit::HumanoidComponent*> players = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			if (player->get_peer()->xplicit_id.as_string() == xid)
			{
				factory.send(Xplicit::COMPONENT_ID_ROXML, _path, Xplicit::NETWORK_REPL_CMD_CREATE, player->get_peer()->public_hash);
			}
		}
	}

	return 0;
}

static int lua_CreateGear(lua_State* L)
{
	const char* name = lua_tostring(L, 1);
	const char* xplicit_id = lua_tostring(L, 2);

	if (xplicit_id == nullptr ||
		name == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	Xplicit::String path_player("World.Players.");
	path_player += xplicit_id;

	Xplicit::GearComponent* gear = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::GearComponent>(name, path_player.c_str());

	if (gear)
	{
		std::vector<Xplicit::HumanoidComponent*> players = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			if (player->get_peer()->xplicit_id.as_string() == xplicit_id)
			{
				player->get_gears().push_back(gear);

				path_player += ".";
				path_player += name;

				lua_pushboolean(L, true);
				return 1;
			}
		}

		Xplicit::ComponentSystem::get_singleton_ptr()->remove(gear);
	}

	lua_pushboolean(L, false);
	return 1;
}

static int lua_DestroyGear(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	auto gear = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::GearComponent>(name);

	if (gear)
	{
		Xplicit::ComponentSystem::get_singleton_ptr()->remove(gear);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushboolean(L, 0);
	return 1;
}

static int lua_Shutdown(lua_State* L)
{
	auto players = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	for (auto& ply : players)
	{
		if (!ply->get_peer())
			continue;

		ply->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] = Xplicit::NETWORK_CMD_SHUTDOWN;
	}

	Xplicit::NetworkServerContext::send_all(Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent"));

	std::exit(0);

	return 0;
}

static bool XPLICIT_IS_SERVER = true;

void XplicitLoadServerLua() noexcept
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService = {}");
	
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_CreateGear, "EngineCreateGear");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService.Create = EngineCreateGear");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyGear, "EngineDestroyGear");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService.Destroy = EngineDestroyGear");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_LoadRoXML, "EngineLoadRoXML");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService.Load = EngineLoadRoXML");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Shutdown, "EngineShutdown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.Shutdown = EngineShutdown");
}
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

		std::vector<Xplicit::HumanoidComponent*> players = Xplicit::ComponentSystem::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			if (player->get_peer()->xplicit_id.as_string() == xid)
			{
				Xplicit::ServerReplicationManager::get_singleton_ptr()->create(Xplicit::COMPONENT_ID_ROXML, _path, player->get_peer()->public_hash);
			}
		}
	}

	return 0;
}

static int lua_CreateGear(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	lua_gettable(L, 2);
	lua_getfield(L, 2, "XplicitId");

	const char* xplicit_id = lua_tostring(L, 2);

	if (xplicit_id == nullptr ||
		name == nullptr)
	{
		lua_pushnil(L);
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
				gear->insert("Owner", path_player.c_str());
				gear->insert("Parent", "Owner");

				player->get_gears().push_back(gear);

				path_player += ".";
				path_player += name;

				lua_getglobal(L, path_player.c_str());
				lua_pushvalue(L, -1);

				return 1;
			}
		}

		Xplicit::ComponentSystem::get_singleton_ptr()->remove(gear);
	}

	lua_pushnil(L);
	return 1;
}

static int lua_DestroyGear(lua_State* L)
{
	lua_rawgeti(L, 1, 1);
	lua_rawgeti(L, 1, 2);

	const char* name = lua_tostring(L, -1);
	const char* xplicit_id = lua_tostring(L, -2);

	auto gear = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::GearComponent>(name);

	if (gear)
		Xplicit::ComponentSystem::get_singleton_ptr()->remove(gear);

	return 0;
}

void XplicitLoadServerLua() noexcept
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService.IsClient = false");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService.IsServer = true");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_CreateGear);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineCreateGear");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService.Create = EngineCreateGear");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_DestroyGear);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineDestroyGear");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.GearService.Destroy = EngineDestroyGear");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineLoadRoXML");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService.Load = EngineLoadRoXML");
}
/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationFactory.h"
#include "HumanoidComponent.h"
#include "GearComponent.h"

#include <CLua.hpp>
#include <RoXML.h>

 // RoXML parser
XPX::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

static int lua_LoadRoXML(lua_State* L)
{
	auto _path = lua_tostring(L, 2);
	auto _client = lua_toboolean(L, 3);

	if (!_client)
	{
		XPX::RoXML::RoXMLDocumentParameters params;
		 
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

		XPX::ServerReplicationFactory factory;
		std::vector<XPX::HumanoidComponent*> players = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::HumanoidComponent>("HumanoidComponent");

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			if (player->get_peer()->xplicit_id.as_string() == xid)
			{
				factory.send(XPX::COMPONENT_ID_ROXML, _path, XPX::NETWORK_REPL_CMD_CREATE, player->get_peer()->public_hash);
			}
		}
	}

	return 0;
}

static int lua_CreateGear(lua_State* L)
{
	const char* name = lua_tostring(L, 2);
	const char* xplicit_id = lua_tostring(L, 3);

	if (xplicit_id == nullptr ||
		name == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	XPX::String path_player("world.Players.");
	path_player += xplicit_id;

	XPX::GearComponent* gear = XPX::ComponentSystem::get_singleton_ptr()->add<XPX::GearComponent>(name, path_player.c_str());

	if (gear)
	{
		std::vector<XPX::HumanoidComponent*> players = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::HumanoidComponent>("HumanoidComponent");

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			if (player->get_gears()[i] == nullptr)
			{
				if (player->get_peer()->xplicit_id.as_string() == xplicit_id)
				{
					player->get_gears()[i] = gear;
					player->get_gears()[i]->set_owner(player);

					path_player += ".";
					path_player += name;

					XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("return {}", path_player).c_str());

					return 1;
				}
			}
		}

		XPX::ComponentSystem::get_singleton_ptr()->remove(gear);
	}

	lua_pushnil(L);
	return 1;
}

static int lua_DestroyGear(lua_State* L)
{
	const char* name = lua_tostring(L, 1);

	auto gear = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::GearComponent>(name);

	if (gear)
	{
		if (gear->get_owner())
		{
			for (size_t i = 0; i < gear->get_owner()->get_gears().size(); i++)
			{
				if (gear->get_owner()->get_gears()[i] == gear)
				{
					gear->get_owner()->get_gears()[i] = nullptr;
					break;
				}
			}
		}

		XPX::ComponentSystem::get_singleton_ptr()->remove(gear);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushboolean(L, 0);
	return 1;
}

static int lua_Shutdown(lua_State* L)
{
	auto players = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::HumanoidComponent>("HumanoidComponent");

	for (auto& ply : players)
	{
		if (!ply->get_peer())
			continue;

		ply->get_peer()->packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] = XPX::NETWORK_CMD_SHUTDOWN;
	}

	XPX::NetworkServerContext::send_all(XPX::ComponentSystem::get_singleton_ptr()->get<XPX::NetworkServerComponent>("NetworkServerComponent"));

	std::exit(0);

	return 0;
}

class XPXInstance
{
public:
	static int new_instance(lua_State* L)
	{
		XPX::String component_name = lua_tostring(L, 1);

		if (component_name == "Gear")
		{
			return lua_CreateGear(L);
		}
		else if (component_name == "Scene")
		{
			return lua_LoadRoXML(L);
		}

		lua_pushnil(L);
		return 1;
	}

};

void XplicitLoadServerLua() noexcept
{
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Shutdown, "Shutdown");

	XPX::RLua::RuntimeClass<XPXInstance> instance;
	instance.begin_class("Instance", &XPXInstance::new_instance).end_class();
}
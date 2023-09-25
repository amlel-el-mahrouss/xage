/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NetworkUtils.h"

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
		params.WaitFor = false;
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

		XPX::NetworkFileTransferFactory factory;
		std::vector<XPX::HumanoidComponent*> players = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::HumanoidComponent>();

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			if (player->get_peer()->xplicit_id.as_string() == xid)
			{
				factory.send(XPX::COMPONENT_ID_ROXML, _path, player->get_peer()->public_hash);
			}
		}
	}

	return 0;
}

static int lua_CreateGear(lua_State* L)
{
	const char* name = lua_tostring(L, 2);
	const char* xplicit_id = lua_tostring(L, 3);
	const char* mesh = lua_tostring(L, 4);

	if (xplicit_id == nullptr ||
		name == nullptr ||
		mesh == nullptr)
	{
		lua_pushnil(L);
		return 1;
	}

	XPX::String path_player = XPLICIT_LUA_NAMESPACE;
	path_player += ".Players.";
	path_player += xplicit_id;

	XPX::GearComponent* gear = XPX::ComponentSystem::get_singleton_ptr()->add<XPX::GearComponent>(name, path_player.c_str());

	if (gear)
	{
		std::vector<XPX::HumanoidComponent*> players = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::HumanoidComponent>();

		for (auto i = 0UL; i < players.size(); ++i)
		{
			auto player = players[i];

			for (auto y = 0UL; player->get_gears().max_size(); ++y)
			{
				if (player->get_gears()[y] == nullptr)
				{
					if (player->get_peer() &&
						player->get_peer()->xplicit_id.as_string() == xplicit_id)
					{
						XPX::NetworkPacket packet = player->get_peer()[y].packet;

						packet.cmd[XPLICIT_NETWORK_CMD_CREATE] = XPX::NETWORK_CMD_CREATE;
						packet.channel = XPLICIT_CHANNEL_GEAR;

						memcpy(packet.replicas[XPLICIT_REPLICA_4], mesh, strlen(mesh));

						player->get_gears()[y] = gear;
						player->get_gears()[y]->set_owner(player);

						path_player += ".";
						path_player += name;

						XPX::NetworkServerContext::send_all(XPX::ComponentSystem::get_singleton_ptr()->get<XPX::NetworkServerComponent>("NetworkServerComponent"), &packet);

						XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("return {}", path_player).c_str());

						return 1;
					}
				}
			}
		}

		XPX::ComponentSystem::get_singleton_ptr()->remove(gear);
	}

	lua_pushnil(L);
	return 1;
}

static int lua_Shutdown(lua_State* L)
{
	auto players = XPX::ComponentSystem::get_singleton_ptr()->all_of<XPX::HumanoidComponent>();

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

class XPXEngineBridge
{
public:
	static const char* name() { return "Engine"; }

public:
	static int new_instance(lua_State* L)
	{
		XPX::String component_name = lua_tostring(L, 1);

		if (component_name == "Gear")
		{
			return lua_CreateGear(L);
		}
		else if (component_name == "RoXML")
		{
			return lua_LoadRoXML(L);
		}

		lua_pushnil(L);
		return 1;
	}

};

static int lua_DestroyGear(lua_State* L)
{
	auto name = lua_tostring(L, 1);
	auto parent = lua_tostring(L, 2);

	if (name && parent)
	{
		XPX::String name_str = name;
		XPX::String parent_str = parent;

		XPX::Thread job([](XPX::String name, XPX::String parent) {
			auto part = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::GearComponent>(name.c_str());

			if (part)
			{
				if (part->get_owner())
				{
					auto* owner = part->get_owner();

					for (size_t i = 0; i < owner->get_gears().size(); i++)
					{
						if (owner->get_gears()[i] == part)
						{
							owner->get_gears()[i] = nullptr;
							break;
						}
					}
				}

				if (part->parent() == parent)
				{
					XPX::ComponentSystem::get_singleton_ptr()->remove(part);
					return;
				}

			}

			XPLICIT_INFO(name + " has not been found on parent: " + parent + ".");
			}, name_str, parent_str);

		job.detach();
	}

	return 0;
}

static int from_scheme(lua_State* L)
{
	auto uri = *reinterpret_cast<XPXUri**>(lua_newuserdata(L, sizeof(XPXUri*))) = new XPXUri();
	uri->Uri = XPX::Utils::UriParser(lua_tostring(L, 1));

	XPLICIT_INFO("XPXUri::on_new");

	return 1;
}

void XplicitLoadServerLua() noexcept
{
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_Shutdown, "shutdown");
	XPX::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_DestroyGear, "destroyGear");

	XPX::RLua::RuntimeClass<XPXUri> uri;
	uri.begin_class().append_proc("from_scheme", from_scheme).append_proc("parse", &XPXUri::parse_url).end_class();

	lua_setglobal(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), XPXUri::name());

	XPX::RLua::RuntimeClass<XPXEngineBridge> instance;
	instance.begin_class().append_proc("new", &XPXEngineBridge::new_instance).end_class();

	lua_setglobal(XPX::Lua::CLuaStateManager::get_singleton_ptr()->state(), XPXEngineBridge::name());
}
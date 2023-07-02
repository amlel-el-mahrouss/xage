/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Corporation, all rights reserved.
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

static int lua_GetHumanoidHealth(lua_State* L)
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
			lua_pushinteger(L, humanoid[index]->get_health());
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int lua_HurtHumanoid(lua_State* L)
{
	const auto humanoid = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (humanoid.empty())
		return 0;

const Xplicit::PlayerID id = lua_tointeger(L, 1);
const int64_t dmg = lua_tointeger(L, 2);

if (id == -1)
return 0;

for (std::size_t index = 0; index < humanoid.size(); index++)
{
	if (humanoid[index]->id() == id)
	{
		auto health = humanoid[index]->get_health();
		health -= dmg;

		humanoid[index]->set_health(health);

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

static int lua_KillHumanoid(lua_State* L)
{
	const auto humanoid = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (humanoid.empty())
		return 0;

	const Xplicit::PlayerID id = lua_tointeger(L, 1);

	for (std::size_t index = 0; index < humanoid.size(); index++)
	{
		if (humanoid[index]->id() == id)
		{
			auto health = humanoid[index]->get_health();
			health -= INT64_MAX;

			humanoid[index]->set_health(health);

			return 0;
		}
	}

	return 0;
}

static int lua_GetXplicitID(lua_State* L)
{
	const auto server = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

	const Xplicit::PlayerID id = lua_tointeger(L, 1);

	if (id > Xplicit::XPLICIT_MAX_CONNECTIONS)
		return 0;

	lua_pushstring(L, server->get(id)->xplicit_id.as_string().c_str());
	return 1;
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

static int lua_AttachScript(lua_State* L)
{
	const auto humanoid = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (humanoid.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);
	auto script = lua_tostring(L, 2);

	for (std::size_t index = 0; index < humanoid.size(); index++)
	{
		if (humanoid[index]->id() == id)
		{
			humanoid[index]->get_attribute().set_script(Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::LuaScriptComponent>(script));
			return 0;
		}
	}

	return 0;
}

void xplicit_load_lua() noexcept
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Humanoid = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_GetHumanoidHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Humanoid:GetHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetHumanoidHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Humanoid:SetHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_HurtHumanoid);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Humanoid:Hurt");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_KillHumanoid);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Humanoid:Kill");

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_LoadScript);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Game:LoadScript");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Game:LoadRoXML");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_GetXplicitID);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Game:GetXplicitID");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_AttachScript);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Game:AttachScript");

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game.Players = {}");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
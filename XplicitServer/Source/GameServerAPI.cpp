/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "ServerReplicationManager.h"
#include "HumanoidComponent.h"

#include <lua/lua.hpp>

static int lua_SetHealth(lua_State* L)
{
	const auto players = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (players.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);
	const int64_t dmg = lua_tointeger(L, 2);

	if (id == -1)
		return 0;

	for (std::size_t index = 0; index < players.size(); index++)
	{
		if (players[index]->id() == id)
		{
			players[index]->health(dmg);
			break;
		}
	}
}

static int lua_GetHealth(lua_State* L)
{
	const auto players = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (players.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);
	const int64_t dmg = lua_tointeger(L, 2);

	if (id == -1)
		return 0;

	for (std::size_t index = 0; index < players.size(); index++)
	{
		if (players[index]->id() == id)
		{
			lua_pushinteger(L, players[index]->health());
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int lua_TakeDamage(lua_State* L)
{
	const auto players = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (players.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);
	const int64_t dmg = lua_tointeger(L, 2);

	if (id == -1)
		return 0;

	for (std::size_t index = 0; index < players.size(); index++)
	{
		if (players[index]->id() == id)
		{
			auto health = players[index]->health();
			health -= dmg;

			players[index]->health(health);

			break;
		}
	}
}

static int lua_PlaySound(lua_State* L)
{
	const char* path = lua_tostring(L, 1);
	const int64_t hash = lua_tointeger(L, 1);

	if (path == nullptr)
		return 1;

	if (hash == XPLICIT_INVALID_HASH)
		return 1;

	// must be an xasset!
	Xplicit::ServerReplicationManager::get_singleton_ptr()->create(Xplicit::COMPONENT_ID_SOUND, path, hash);

	return 0;
}

static int lua_Kill(lua_State* L)
{
	const auto players = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (players.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);

	for (std::size_t index = 0; index < players.size(); index++)
	{
		if (players[index]->id() == id)
		{
			auto health = players[index]->health();
			health -= INT64_MAX; // lol

			players[index]->health(health);

			break;
		}
	}

	return 0;
}

static int lua_NetworkPacketFromPlayerID(lua_State* L)
{
	const auto player_list = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	const Xplicit::String id = lua_tostring(L, 1);

	for (auto* player : player_list)
	{
		if (player->get_peer()->xplicit_id.as_string() != id)
			continue;

		auto phash = player->get_peer()->public_hash;
		auto health = player->get_peer()->packet.health;
		auto channel = player->get_peer()->packet.channel;

		auto pos = player->get_peer()->packet.pos;
		auto cmd = player->get_peer()->packet.cmd;

		int16_t cmds[XPLICIT_NETWORK_CMD_MAX];
		memcpy(cmds, cmd, XPLICIT_NETWORK_CMD_MAX);

		Xplicit::NetworkFloat poses[XPLICIT_NETWORK_POS_MAX];
		memcpy(poses, pos, XPLICIT_NETWORK_POS_MAX);

		lua_newtable(L);
		int top = lua_gettop(L);

		lua_pushinteger(L, phash);
		lua_pushinteger(L, health);
		lua_pushinteger(L, channel);

		for (size_t i = 0; i < XPLICIT_NETWORK_CMD_MAX; ++i)
		{
			lua_pushinteger(L, cmds[i]);
		}

		for (size_t i = 0; i < XPLICIT_NETWORK_POS_MAX; ++i)
		{
			lua_pushnumber(L, poses[i]);
		}

		lua_settable(L, top);

		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int lua_NetworkGetXplicitID(lua_State* L)
{
	const auto server = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

	const std::int64_t id = lua_tointeger(L, 1);

	if (id > Xplicit::XPLICIT_MAX_CONNECTIONS)
		return 0;

	lua_pushstring(L, server->get(id)->xplicit_id.as_string().c_str());
	return 1;
}

void xplicit_load_lua() noexcept
{
	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_GetHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "GetPlayerHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "SetPlayerHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_TakeDamage);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "HurtPlayer");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_Kill);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "KillPlayer");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "PlaySound");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_NetworkPacketFromPlayerID);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "GetPacketFromPlayerID");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_NetworkGetXplicitID);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "GetXplicitID");

	XPLICIT_GET_DATA_DIR(full_path);

	full_path += "Contents/";
	full_path += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(full_path.c_str());
}
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

void xplicit_register_server_lua()
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("PlayerManager = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_GetHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "PlayerManager:GetHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "PlayerManager:SetHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_TakeDamage);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "PlayerManager:Hurt");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_Kill);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "PlayerManager:Kill");

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("Sound = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Sound:Play");

	XPLICIT_GET_DATA_DIR(full_path);

	full_path += "Contents/";
	full_path += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(full_path.c_str());
}
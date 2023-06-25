/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

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

void xplicit_register_server_lua()
{
	lua_pushcfunction(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), lua_GetHealth);
	lua_setglobal(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), "PlayerGetHealth");

	lua_pushcfunction(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), lua_SetHealth);
	lua_setglobal(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), "PlayerSetHealth");

	lua_pushcfunction(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), lua_TakeDamage);
	lua_setglobal(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), "PlayerTakeDamage");

	XPLICIT_GET_DATA_DIR(fullPath);

	fullPath += "Contents/";
	fullPath += "xplicit-server-api.lua";

	luaL_dofile(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), fullPath.c_str());
}
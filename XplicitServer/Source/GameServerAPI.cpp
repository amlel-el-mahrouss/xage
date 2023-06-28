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

static int lua_SetHumanoidHealth(lua_State* L)
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

	return 0;
}

static int lua_GetHumanoidHealth(lua_State* L)
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

static int lua_HurtHumanoid(lua_State* L)
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

	return 0;
}

static int lua_LoadScript(lua_State* L)
{
	const char* path = lua_tostring(L, 1);
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
	const auto players = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

	if (players.empty())
		return 0;

	const int64_t id = lua_tointeger(L, 1);

	for (std::size_t index = 0; index < players.size(); index++)
	{
		if (players[index]->id() == id)
		{
			auto health = players[index]->health();
			health -= INT64_MAX;

			players[index]->health(health);

			break;
		}
	}

	return 0;
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
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("Humanoid = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_GetHumanoidHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Humanoid:GetHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetHumanoidHealth);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Humanoid:SetHealth");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_HurtHumanoid);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Humanoid:Hurt");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_KillHumanoid);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Humanoid:Kill");

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("Game = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_LoadScript);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Game:LoadScript");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_NetworkGetXplicitID);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Game:GetID");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(tmp.c_str());

	tmp.clear();

	tmp = full_path;

	tmp += "Contents/";
	tmp += "autorun.lua";

	if (std::filesystem::exists(tmp))
		Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
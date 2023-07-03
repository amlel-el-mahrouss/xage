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
#include "InstanceComponent.h"

#include <lua/lua.hpp>
#include <RoXML.h>

static int lua_CreateComponent(lua_State* L)
{
	Xplicit::String comp_name = lua_tostring(L, 1);

	if (comp_name.empty())
		return 0;

	if (comp_name == "Spawn")
	{
		float x = lua_tointeger(L, 2);
		float y = lua_tointeger(L, 3);
		float z = lua_tointeger(L, 4);

		float pos[3] = { x, y, z };

		auto pos_component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::InstanceComponent>();
		
		pos_component->get_attribute().pos().X = x;
		pos_component->get_attribute().pos().Y = y;
		pos_component->get_attribute().pos().Z = z;

		pos_component->get_attribute().script(Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::LuaScriptComponent>("xplicit_base_spawn.lua"));

		return 0;
	}

	if (comp_name == "Team")
	{
		Xplicit::String team_name = lua_tostring(L, 2);

		auto pos_component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::InstanceComponent>();
		pos_component->get_attribute().script(Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::LuaScriptComponent>("xplicit_base_team.lua"));

		Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string(std::format("Engine:GetLastestTeam().Construct('{}')", team_name).c_str());

		return 0;
	}

	if (comp_name == "Tool")
	{
		Xplicit::String tool_name = lua_tostring(L, 2);

		auto pos_component = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::InstanceComponent>();
		pos_component->get_attribute().script(Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::LuaScriptComponent>("xplicit_base_tool.lua"));

		Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string(std::format("Engine:GetLastestTool().Construct('{}')", tool_name).c_str());

		return 0;
	}

	if (comp_name == "Humanoid")
	{
		int comp_id = lua_tointeger(L, 2);

		Xplicit::Thread job([](int _comp_id) {
			const auto vec_humans = Xplicit::ComponentManager::get_singleton_ptr()->all_of<Xplicit::HumanoidComponent>("HumanoidComponent");

			for (size_t i = 0; i < vec_humans.size(); i++)
			{
				if (vec_humans[i]->id() == _comp_id)
					return;
			}

			auto human = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::HumanoidComponent>();

			human->set_health(100);
			human->set_peer(nullptr);
			human->set_id(_comp_id);
		}, comp_id);

		job.detach();

		return 0;
	}

	return 0;
}

void xplicit_load_lua() noexcept
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Component = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Game.Players = {}");

	XPLICIT_GET_DATA_DIR(full_path);

	Xplicit::String tmp = full_path;
	tmp += "Contents/";
	tmp += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(tmp.c_str());
}
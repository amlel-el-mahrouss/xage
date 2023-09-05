/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"
#include "SoundComponent.h"
#include "GameMenuUI.h"
#include "Application.h"
#include "Mesh.h"
#include "FX.h"

#include <XplicitSound.h>
#include <CLua/CLua.hpp>
#include <codecvt>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>

 // RoXML parser
Xplicit::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

#ifndef XPLICIT_XASSET_IDENT
#	define XPLICIT_XASSET_IDENT ("xasset")
#endif // ifndef XPLICIT_XASSET_IDENT

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_TO_WCHAR;

static int lua_PlaySound(lua_State* L)
{
	try
	{
		Xplicit::String url = lua_tostring(L, 1);

		if (url.empty() ||
			url.find(XPLICIT_XASSET_IDENT) == Xplicit::String::npos)
			return 0;

		Xplicit::String substr = url.erase(url.find(XPLICIT_XASSET_IDENT), strlen(XPLICIT_XASSET_IDENT) + 3);

		url.clear();
		url = "/";
		url += substr;

		std::cout << url << std::endl;

		static Xplicit::Player::LocalNetworkMonitorEvent* monitor = Xplicit::EventSystem::get_singleton_ptr()->get<Xplicit::Player::LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

		if (!monitor)
			monitor = Xplicit::EventSystem::get_singleton_ptr()->get<Xplicit::Player::LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

		Xplicit::String endpoint = XPLICIT_XASSET_ENDPOINT;
		monitor->HTTP->set_endpoint(endpoint);

		auto tmp = std::to_string(xplicit_get_epoch()) + "-tmp-snd.wav";

		if (monitor &&
			monitor->HTTP->download(url, tmp))
		{
			XPLICIT_GET_DATA_DIR(full_path);

			Xplicit::String full_download_path;

			full_download_path += full_path;
			full_download_path += "Contents/";
			full_download_path += tmp;

			if (auto snd = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::Player::SoundComponent>("SoundComponent"))
				snd->play(full_download_path);
		}

	}
	catch (...)
	{
		XPLICIT_CRITICAL("There was an error executing this procedure!");
	}

	return 0;
}

static int lua_LoadRoXML(lua_State* L)
{
	auto _path = lua_tostring(L, 1);

	Xplicit::RoXML::RoXMLDocumentParameters params;

	params.Has3D = true;
	params.NoLua = true;
	params.LuaOnly = false;

	params.Path = _path;

	if (params.Path.empty())
	{
		lua_pushboolean(L, false);
		return 1;
	}

	XPLICIT_PARSER.parse(params);

	lua_pushboolean(L, true);
	return 1;
}

static int lua_LoadModel(lua_State* L)
{
	auto _path = lua_tostring(L, 1);
	auto _name = lua_tostring(L, 2);
	auto _parent = lua_tostring(L, 3);

	Xplicit::Player::StaticMesh* mesh = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::StaticMesh>(
		_path, 
		_name, 
		_parent);

	return 0;
}

static int lua_MakeRect(lua_State* L)
{
	const char* parent = lua_tostring(L, 1);
	const char* name = lua_tostring(L, 2);

	Xplicit::Player::LocalFrameComponent* frame = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::LocalFrameComponent>(parent, name);

	lua_getglobal(L, (Xplicit::String(parent) + "." + name).c_str());
	lua_pushvalue(L, -1);

	return 0;
}

static int lua_KeyDown(lua_State* L)
{
	lua_pushboolean(L, KB->key_down());

	return 1;
}

static int lua_IsKeyDown(lua_State* L)
{
	int key = lua_tointeger(L, 1);

	lua_pushboolean(L, KB->key_down(key));

	return 1;
}

static int lua_IsLeftDown(lua_State* L)
{
	lua_pushboolean(L, KB->left_down());

	return 1;
}

static int lua_IsRightDown(lua_State* L)
{
	lua_pushboolean(L, KB->right_down());

	return 1;
}

static int lua_ExplodeFX(lua_State* L)
{
	// position

	lua_rawgeti(L, 1, 1);
	lua_rawgeti(L, 1, 2);
	lua_rawgeti(L, 1, 3);

	float z_pos = lua_tonumber(L, -3);
	float y_pos = lua_tonumber(L, -2);
	float x_pos = lua_tonumber(L, -1);

	auto pos = irr::core::vector3df(x_pos, y_pos, z_pos);
	auto scale = irr::core::vector3df(5, 5, 5);

	Xplicit::Explosion explode(pos, scale);

	return 0;
}

void XplicitLoadClientLua() noexcept
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.MeshService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.UIService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.SoundService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.InputService = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService.IsClient = true");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService.IsServer = false");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineLoadRoXML");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService.Load = EngineLoadRoXML");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EnginePlaySound");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.SoundService.Play = EnginePlaySound");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_MakeRect);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineMakeRect");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.UIService.MakeRect = EngineMakeRect");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_KeyDown);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineKeyDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.InputService.KeyDown = EngineKeyDown");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_IsKeyDown);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineIsKeyDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.InputService.IsKeyDown = EngineIsKeyDown");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_IsLeftDown);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineIsLeftDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.InputService.IsLeftDown = EngineIsLeftDown");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_IsRightDown);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineIsRightDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.InputService.IsRightDown = EngineIsRightDown");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadModel);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineLoadMesh");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.MeshService.Load = EngineLoadMesh");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_ExplodeFX);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "__EngineRPCExplodeFX");

	Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::SoundComponent>();
}

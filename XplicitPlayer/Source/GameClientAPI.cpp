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
#include "Application.h"
#include "Mesh.h"

#include <XplicitSound.h>
#include <CLua/CLua.hpp>
#include <codecvt>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>

 // RoXML parser
Xplicit::RoXML::RoXMLDocumentParser XPLICIT_PARSER;

#ifdef XPLICIT_WINDOWS

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_TO_WCHAR;

static int lua_PlaySound(lua_State* L)
{
	try
	{
		Xplicit::String path = lua_tostring(L, 1);

		if (path.empty())
			return 0;

		if (auto snd = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::Player::SoundComponent>("SoundComponent"))
			snd->play(path);
	}
	catch (...)
	{
		XPLICIT_CRITICAL("There was an error executing this procedure!");
	}

	return 0;
}

static int lua_SetWindowCaption(lua_State* L)
{
	auto title = lua_tostring(L, -1);

	if (title)
		RENDER->setWindowCaption(XPLICIT_TO_WCHAR.from_bytes(title).c_str());

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

	Xplicit::Player::StaticMesh* mesh = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::StaticMesh>(_path, _name, _parent);

	return 0;
}

void XplicitLoadClientLua() noexcept
{
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RenderingService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.SoundService = {}");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService = {}");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService.IsClient = true");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RunService.IsServer = false");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_LoadRoXML);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineLoadRoXML");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RoXMLService.Load = EngineLoadRoXML");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EnginePlaySound");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.SoundService.Play = EnginePlaySound");

	lua_pushcfunction(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), lua_SetWindowCaption);

	lua_setglobal(Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state(), "EngineSetWindowCaption");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("_G.World.RenderingService.SetCaption = EngineSetWindowCaption");
	
	Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::SoundComponent>();
}

#endif // ifdef XPLICIT_WINDOWS
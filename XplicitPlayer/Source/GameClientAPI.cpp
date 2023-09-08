/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"
#include "SoundComponent.h"
#include "GameMenuUI.h"
#include "Application.h"
#include "ClientUtils.h"
#include "Mesh.h"

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

#define XPLICIT_ALLOWED_EXTENSION ".wav"

static int lua_PlaySound(lua_State* L)
{
	try
	{
		Xplicit::String url = lua_tostring(L, 1);

		// that means if we don't find it, then fail silently.
		if (url.find(XPLICIT_ALLOWED_EXTENSION) == Xplicit::String::npos)
			return 0;

		if (url.empty() ||
			url.find(XPLICIT_XASSET_IDENT) == Xplicit::String::npos)
			return 0;

		Xplicit::String substr_tmp = url.erase(url.find(XPLICIT_XASSET_IDENT), strlen(XPLICIT_XASSET_IDENT) + 3);

		url.clear();
		url = "/";
		url += substr_tmp;

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

static int lua_MakeSoundComponent(lua_State* L)
{
	if (lua_tostring(L, 1) &&
		lua_tostring(L, 2))
	{
		auto name = lua_tostring(L, 1);
		auto parent = lua_tostring(L, 2);

		Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::SoundComponent>(name, parent);
	}
}

void XplicitLoadClientLua() noexcept
{
	auto L = Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->state();

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_LoadRoXML, "XPXLoadScene");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_PlaySound, "XPXPlaySound");

	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_MakeRect, "XPXMakeRect");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_KeyDown, "XPXKeyDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsKeyDown, "XPXIsKeyDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsLeftDown, "XPXIsLeftDown");
	Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->global_set(lua_IsRightDown, "XPXIsRightDown");

}

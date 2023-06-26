/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: WinMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"
#include "Application.h"

#include <LuaScriptComponent.h>
#include <CommonEngine.h>
#include <XplicitSound.h>
#include <DataValue.h>
#include <lua/lua.hpp>
#include <Xplicit.h>
#include <codecvt>
#include <Uri.h>

static Xplicit::Player::LocalNetworkMonitorEvent* XPLICIT_MONITOR;

#ifdef XPLICIT_WINDOWS

static int lua_PlaySound(lua_State* L)
{
	const char* path = lua_tostring(L, 1);

	if (path == nullptr)
		return 0;

	std::wstring assetPath;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	assetPath = converter.from_bytes(path);

	auto aud = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(assetPath.c_str());

	if (aud)
		aud->play();

	return 0;
}

void xplicit_load_lua()
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("Sound = {};");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Sound:Play");
}

#endif // ifdef XPLICIT_WINDOWS
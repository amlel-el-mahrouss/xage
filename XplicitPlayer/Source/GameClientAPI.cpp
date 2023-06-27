/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
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

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_UTF16_CONV;

#ifdef XPLICIT_WINDOWS

static int lua_PlaySound(lua_State* L)
{
	Xplicit::String path = lua_tostring(L, 1);

	if (path.empty())
		return 0;
	
	auto aud = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(XPLICIT_UTF16_CONV.from_bytes(path).c_str());

	if (aud)
		aud->play();

	return 0;
}

static int lua_SetTitle(lua_State* L)
{
	Xplicit::String path = lua_tostring(L, 1);

	if (path.empty())
		return 0;

	RENDER->setWindowCaption(XPLICIT_UTF16_CONV.from_bytes(path).c_str());

	return 0;
}

void xplicit_load_lua() noexcept
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("Sound = {};");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Sound:Play");
	
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("Window = {};");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetTitle);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "Window:SetTitle");

	XPLICIT_GET_DATA_DIR(full_path);

	full_path += "Contents/";
	full_path += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(full_path.c_str());
}

#endif // ifdef XPLICIT_WINDOWS
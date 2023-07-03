/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalNetworkMonitorEvent.h"
#include "LocalSoundComponent.h"
#include "Application.h"

#include <XplicitSound.h>
#include <lua/lua.hpp>
#include <codecvt>
#include <Util.h>
#include <Uri.h>

#ifdef XPLICIT_WINDOWS

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> XPLICIT_UTF16_CONV;

static int lua_PlaySound(lua_State* L)
{
	Xplicit::String path = lua_tostring(L, 1);

	if (path.empty())
		return 0;
	
	if (auto snd = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::Player::LocalSoundComponent>("LocalSoundComponent"))
		snd->play_2d(path);

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
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Sound = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Sound:Play");

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Window = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_SetTitle);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "_G.Window:SetCaption");

	XPLICIT_GET_DATA_DIR(full_path);

	full_path += "Contents/";
	full_path += "xplicit.lua";

	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run(full_path.c_str());
}

#endif // ifdef XPLICIT_WINDOWS
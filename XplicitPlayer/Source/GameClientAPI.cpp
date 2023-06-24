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

#include <CommonEngine.h>
#include <XplicitSound.h>
#include "Application.h"
#include <lua/lua.hpp>
#include <Xplicit.h>
#include <codecvt>
#include <Uri.h>

static Xplicit::Player::LocalNetworkMonitorEvent* XPLICIT_MONITOR;

#ifdef XPLICIT_WINDOWS

static Xplicit::Audio::XAudioEngine XPLICIT_AUDIO;

static int lua_PlaySound(lua_State* L)
{
	// xcasset://Halloween.wav

	const char* path = lua_tostring(L, 1);

	if (!path)
	{
		XPLICIT_CRITICAL("Xplicit asset has not been specified!");
		return 0;
	}

	std::unique_ptr<Xplicit::Utils::UriParser> uri = std::make_unique<Xplicit::Utils::UriParser>(path);

	if (uri->protocol() != "xasset://")
	{
		XPLICIT_CRITICAL("Not an Xasset!");
		return 0;
	}

	XPLICIT_MONITOR->HTTP->download(uri->get());

	XPLICIT_GET_DATA_DIR(fullPath);

	fullPath += "Contents/";
	fullPath += uri->get();

	if (!std::filesystem::exists(fullPath))
	{
		XPLICIT_CRITICAL("Failed to download content from HTTP!");
		return 0;
	}

	std::wstring assetPath;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	assetPath = converter.from_bytes(fullPath);

	auto aud = XPLICIT_AUDIO.make_audio(assetPath.c_str());

	if (aud)
		(*aud)();

	return 0;
}

void xplicit_register_clientside_win64()
{
	lua_pushcfunction(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::ILuaStateManager::get_singleton_ptr()->state(), "PlaySound");
}

#endif // ifdef XPLICIT_WINDOWS
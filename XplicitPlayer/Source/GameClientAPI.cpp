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

static int lua_PlaySound(lua_State* L)
{
	try
	{
		Xplicit::String path = lua_tostring(L, 1);

		if (path.empty())
			return 0;

		if (auto snd = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::Player::LocalSoundComponent>("LocalSoundComponent"))
			snd->play(path);
	}
	catch (...)
	{
		XPLICIT_CRITICAL("There was an error executing this procedure!");
	}

	return 0;
}

void XplicitLoadClientLua() noexcept
{
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.RenderingService = {}");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.SoundService = {}");

	lua_pushcfunction(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), lua_PlaySound);
	lua_setglobal(Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->state(), "CoreAPI_PlaySound");
	Xplicit::Lua::XLuaStateManager::get_singleton_ptr()->run_string("_G.Engine.SoundService.Play = CoreAPI_PlaySound");

	Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalSoundComponent>();
}

#endif // ifdef XPLICIT_WINDOWS